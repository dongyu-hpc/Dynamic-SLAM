/** -----------------------------------------------------------------------------
 * Copyright (c) 2024 Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * AUTHORS:      Lukas Schmid <lschmid@mit.edu>, Marcus Abate <mabate@mit.edu>,
 *               Yun Chang <yunchang@mit.edu>, Luca Carlone <lcarlone@mit.edu>
 * AFFILIATION:  MIT SPARK Lab, Massachusetts Institute of Technology
 * YEAR:         2024
 * SOURCE:       https://github.com/MIT-SPARK/Khronos
 * LICENSE:      BSD 3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * -------------------------------------------------------------------------- */


#include "khronos/backend/change_detection/sequential_change_detector.h"
#include <cstdlib>

namespace khronos {

void declare_config(SequentialChangeDetector::Config& config) {
  using namespace config;
  name("SequentialChangeDetector");

  // Member configs.
  field(config.ray_verificator, "ray_verificator");
  field(config.ray_change_detector, "ray_change_detector");

  config.objects.setOptional();
  field(config.objects, "objects");
  config.background.setOptional();
  field(config.background, "background");
}

SequentialChangeDetector::SequentialChangeDetector(const Config& config)
    : config(config::checkValid(config)) {
  // Setup members.
  ray_verificator_ = std::make_shared<RayVerificator>(config.ray_verificator);
  ray_change_detector_ = std::make_shared<RayChangeDetector>(config.ray_change_detector);
  object_change_detector_ = config.objects.create(ray_verificator_, ray_change_detector_);
  if (!object_change_detector_) {
    object_change_detector_ = std::make_unique<ObjectChangeDetector>();
  }
  background_change_detector_ = config.background.create(ray_verificator_, ray_change_detector_);
  if (!background_change_detector_) {
    background_change_detector_ = std::make_unique<BackgroundChangeDetector>();
  }
  
  // Instrumentation output path. Set KHRONOS_RV_HIST_CSV to enable histogram dumping.
  if (const char* path = std::getenv("KHRONOS_RV_HIST_CSV")) {
    rv_hist_csv_ = path;
    LOG(INFO) << "[CD] Writing ray verificator histograms to '" << rv_hist_csv_ << "'.";
  }
}

void SequentialChangeDetector::setDsg(std::shared_ptr<const DynamicSceneGraph> dsg) {
  dsg_ = std::move(dsg);
  ray_verificator_->setDsg(dsg_);
}

const Changes& SequentialChangeDetector::detectChanges(const RPGOMerges& rpgo_merges,
                                                      TimeStamp stamp,
                                                      bool had_loopclosure) {
  const std::string timer_suffix = had_loopclosure ? "_recompute" : "_incremental";
  Timer timer("change_detection" + timer_suffix + "/all", stamp);

  // Label the ray verificator's internal timers with the same stamp as the backend.
  ray_verificator_->setTimerStamp(stamp);
  ++num_detect_calls_;

  // Tag used for the instrumentation log lines. Keeps recompute and incremental spins separable,
  // since a loop closure rebuilds the hash and re-queries every change from scratch.
  const std::string mode = had_loopclosure ? "recompute" : "incremental";

  // Setup the ray verificator.
  Timer detail_timer("change_detection" + timer_suffix + "/update_ray_verificator", stamp);
  ray_verificator_->updateDsg();

  // In case of loop closure, recompute the spatial hash and all changes.
  if (had_loopclosure) {
    ray_verificator_->recomputeHash();
    changes_.object_changes.clear();
    changes_.background_changes.clear();
  }

  // Perform object-level change detection.
  detail_timer.reset("change_detection" + timer_suffix + "/objects");
  ray_verificator_->resetStats();
  object_change_detector_->detectChanges(*dsg_, rpgo_merges, changes_.object_changes);
  ray_verificator_->logQueryStats(mode + "/objects");

  // Perform background-level change detection.
  detail_timer.reset("change_detection" + timer_suffix + "/background");
  ray_verificator_->resetStats();
  background_change_detector_->detectChanges(*dsg_, changes_.background_changes);
  ray_verificator_->logQueryStats(mode + "/background");

  // Periodically dump the full rays-per-block histogram. O(num_blocks), so not every spin.
  if (!rv_hist_csv_.empty() && (had_loopclosure || num_detect_calls_ % 10 == 0)) {
    ray_verificator_->logStructureStats(mode + "/periodic", rv_hist_csv_);
  }

  LOG(INFO) << "[CD/" << mode << "] call=" << num_detect_calls_
            << " stamp=" << stamp
            << " object_changes=" << changes_.object_changes.size()
            << " background_changes=" << changes_.background_changes.size();

  return changes_;
}

}  // namespace khronos

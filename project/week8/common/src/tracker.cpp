/**
 * @file tracker.cpp
 * @brief The methods of Tracker that are not defined in the class.
 * @author Zeid Kootbally
 */
#include "tracker.hpp"

#include <algorithm>

int Tracker::add(Pose pose, double speed, bool is_static) {
  const int id{next_id_++};
  tracks_.push_back(std::make_unique<Obstacle>(id, pose, speed, is_static));
  return id;
}

bool Tracker::lose(int id) {
  const auto removed{std::erase_if(
      tracks_, [id](const std::unique_ptr<Obstacle>& track) { return track->id == id; })};
  return removed > 0;
}

const Obstacle* Tracker::find(int id) const {
  for (const std::unique_ptr<Obstacle>& track : tracks_) {
    if (track->id == id) {
      return track.get();
    }
  }
  return nullptr;
}

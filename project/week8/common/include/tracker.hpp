/**
 * @file tracker.hpp
 * @brief The tracker of L7, as a class: it owns one Obstacle for each obstacle
 *        it sees.
 * @author Zeid Kootbally
 *
 * @details The invariant: no two Obstacle objects the tracker holds have the
 * same id. add() gives each new Obstacle the next id, so code outside the class
 * never picks one. The ids come from a static counter, shared by every Tracker,
 * so two trackers (a front camera's and a rear camera's) never hand out the
 * same id either.
 *
 * Tracker defines none of the five special members: its only data member, a
 * std::vector of std::unique_ptr, gives back the memory itself (the rule of
 * zero). A Tracker can be moved and cannot be copied, because a
 * std::unique_ptr cannot be copied.
 */
#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "obstacle.hpp"

/// Owns the Obstacle objects of the obstacles the ego vehicle sees.
class Tracker {
 public:
  /**
   * @brief Starts tracking a new obstacle.
   * @param pose Where it is.
   * @param speed In m/s.
   * @param is_static True when it does not move.
   * @return The id the new Obstacle gets.
   */
  int add(Pose pose, double speed, bool is_static = false);

  /**
   * @brief Stops tracking an obstacle, and deletes its Obstacle.
   * @param id The id add() returned.
   * @return False when no Obstacle has that id.
   */
  bool lose(int id);

  /**
   * @brief Finds an obstacle by id.
   * @return A pointer that does not own the Obstacle (L7), or nullptr.
   */
  [[nodiscard]] const Obstacle* find(int id) const;

  /// How many obstacles are tracked.
  [[nodiscard]] std::size_t size() const noexcept { return tracks_.size(); }

  /// The id the next add() gives, in every Tracker.
  [[nodiscard]] static int next_id() noexcept { return next_id_; }

 private:
  std::vector<std::unique_ptr<Obstacle>> tracks_;  ///< the only owner of each
  inline static int next_id_{1};  ///< one counter for the whole class
};

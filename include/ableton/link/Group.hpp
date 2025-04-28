#pragma once

#include <ableton/link/Beats.hpp>
#include <algorithm>
#include <array>
#include <chrono>
#include <tuple>

namespace ableton::link
{

using GroupId = std::array<std::uint8_t, 15>;

#if 0
struct GroupId : std::array<char, 15>
{
	using std::array<char, 15>::array;

	inline bool operator==(GroupId &gid) const
	{
		return std::equal(begin(), end(), gid.begin(), gid.end());
	}
};
#endif

struct GroupState
{
  static const std::int32_t key = 'grup';
  static_assert(key == 0x67727570, "Unexpected byte order");

  using GroupStateTuple = std::tuple<bool, Beats, std::chrono::microseconds, GroupId>;
  bool isPlaying{false};
  Beats beats{0.};
  std::chrono::microseconds timestamp{0};
  GroupId group_id;

  bool operator==(GroupState const&) const = default;

  // Model the NetworkByteStreamSerializable concept
  friend std::uint32_t sizeInByteStream(const GroupState& state)
  {
    return discovery::sizeInByteStream(state.asTuple());
  }

  template <typename It>
  friend It toNetworkByteStream(const GroupState& state, It out)
  {
    return discovery::toNetworkByteStream(state.asTuple(), std::move(out));
  }

  template <typename It>
  static std::pair<GroupState, It> fromNetworkByteStream(It begin, It end)
  {
    using namespace std;
    using namespace discovery;
    auto result = Deserialize<GroupStateTuple>::fromNetworkByteStream(
      std::move(begin), std::move(end));
    auto state = GroupState{get<0>(result.first), get<1>(result.first),
      get<2>(result.first), get<3>(result.first)};
    return make_pair(std::move(state), std::move(result.second));
  }

  GroupStateTuple asTuple() const
  {
    return {isPlaying, beats, timestamp, group_id};
  }
};

struct ClientGroupState
{
  bool isPlaying{false};
  std::chrono::microseconds time{0};
  std::chrono::microseconds timestamp{0};
  GroupId group_id;

  bool operator==(ClientGroupState const&) const = default;
};

struct ApiGroupState
{
  bool isPlaying{false};
  std::chrono::microseconds time{0};
  GroupId group_id;

  bool operator==(ApiGroupState const&) const = default;
};

} // namespace ableton::link

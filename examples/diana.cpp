#include <ableton/Link.hpp>

int main()
{
  ableton::Link link(120.0);
  link.enable(true);
  link.enableStartStopSync(true);

  {
    auto state = link.captureAudioSessionState();
    state.setIsPlayingInGroup(true, link.clock().micros(), "x1");
    link.commitAudioSessionState(state);
  }


  for (int i = 0; i < 30; ++i)
  {
    auto state = link.captureAudioSessionState();
    auto beat = state.beatAtTime(link.clock().micros(), 4);
    auto group = state.isPlayingInGroup();

    if (group)
    {
      std::cout << "Playing = " << *group;
    }
    else
    {
      std::cout << "Not playing";
    }

    std::cout << ", Beat = " << beat << std::endl;
    sleep(1);
  }

  {
    auto state = link.captureAudioSessionState();
    state.setIsPlayingInGroup(false, link.clock().micros(), "x1");
    link.commitAudioSessionState(state);
  }

  return 0;
}

#include "Sound.h"
#include "Settings.h"
#include <thread>
#include <atomic>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

static int ClampInt(int v, int lo, int hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static void Tone(int freq, int ms)
{
    if (!gSettings.useSound) return;

#ifdef _WIN32
    freq = ClampInt(freq, 37, 32767);
    ms   = ClampInt(ms, 5, 2000);

    std::thread([freq, ms]() {
        if (!::Beep(freq, ms)) \
        {
            ::MessageBeep(MB_OK);
            ::Sleep(ms);
        }
    }).detach();
#else
    (void)freq; (void)ms;
#endif
}

static void Sweep(int startFreq, int endFreq, int steps, int totalMs)
{
    if (steps <= 0) return;
    int per = totalMs / steps;
    if (per < 8) per = 8;

    for (int i = 0; i < steps; ++i) {
        const double t = (steps == 1) ? 1.0 : (double)i / (double)(steps - 1);
        const int f = (int)(startFreq + (endFreq - startFreq) * t);
        Tone(f, per);
    }
}


static bool AllowEveryMs(int ms)
{
    static std::atomic<long long> last{0};
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();

    long long prev = last.load();
    if (now - prev < ms) return false;
    last.store(now);
    return true;
}


namespace Sound
{
    // Menu
    void TickMenuNav()    { Tone(900,  25); }
    void TickMenuSelect() { Tone(1400, 45); }
    void TickMenuToggle() { Tone(1600, 35); Tone(1200, 25); }

    // Collisions / invalid
    void ThumpWall()      { Tone(220,  70); }
    void InvalidAction()  { Tone(180,  90); Tone(140, 70); }

    // Spring
    void SpringCompress(int k)
    {
        if (!AllowEveryMs(35)) return; // ✅
        k = ClampInt(k, 1, 10);
        const int base = 650 + k * 70;
        Tone(base, 18);
        Tone(base + 120, 22);
    }


    void SpringRelease(int k)
    {
        k = ClampInt(k, 1, 10);
        const int hi = 1100 + k * 120;
        Sweep(700, hi, 4, 120 + k * 10);
        Sweep(hi, 600, 3, 90);
    }

    // Doors
    void DoorOpen()       { Tone(520, 30); Tone(780, 40); }
    void DoorClose()      { Tone(260, 55); }
    void DoorWinOpen()    { Tone(1200, 40); Tone(1500, 55); Tone(1800, 60); }

    // Pickups / bombs
    void PickupKey()      { Tone(1200, 35); Tone(1600, 45); }
    void PickupTorch()    { Sweep(500, 900, 3, 90); }

    void BombPlant()      { Tone(800, 25); Tone(700, 25); }

    // יש לך כבר שימוש בקוד בשם BombExplosion(), אז נשמור את השם הזה
    void BombExplode()
    {
        // BOOM עמום (לא חזק מדי)
        Tone(140, 80);
        Tone(110, 90);
        Tone(90,  110);
    }

    void BombExplosion()
    {
        // alias לשם הישן
        BombExplode();
    }

    // Riddle
    void RiddleCorrect()  { Tone(1000, 40); Tone(1500, 60); }
    void RiddleWrong()    { Tone(220,  120); Tone(180, 120); }
}
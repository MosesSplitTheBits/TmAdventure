#pragma once

namespace Sound
{
    // Menu
    void TickMenuNav();
    void TickMenuSelect();
    void TickMenuToggle();

    // Movement / collisions (רק אירועים משמעותיים)
    void ThumpWall();        // ניסיון להיכנס בקיר/חסימה
    void InvalidAction();    // פעולה לא חוקית כללית

    // Spring (# compression / release)
    // k = כמה "דחיסה" (או עוצמה/שלב), ככל שגדול יותר -> pitch גבוה/ארוך יותר
    void SpringCompress(int k);
    void SpringRelease(int k);

    // Doors
    void DoorOpen();
    void DoorClose();
    void DoorWinOpen();      // דלת ניצחון (צליל יותר "בהיר")

    // Pickups / bombs
    void PickupKey();
    void PickupTorch();
    void BombPlant();

    // תן לשני השמות לעבוד (תאימות לקוד ישן/חדש)
    void BombExplode();
    void BombExplosion();

    // Riddle / puzzle feedback
    void RiddleCorrect();
    void RiddleWrong();
}
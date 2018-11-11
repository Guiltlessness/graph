#pragma once
struct MouseEvent
{
    int xrel;
    int yrel;
    bool lmb, rmb;
};
bool pollMouseEvent(MouseEvent &) noexcept;

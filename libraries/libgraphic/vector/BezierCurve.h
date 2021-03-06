#pragma once

#include <libutils/Vec2.h>

namespace graphic
{

struct BezierCurve
{
    Vec2f start;
    Vec2f first_control_point;
    Vec2f second_contol_point;
    Vec2f end;
};

} // namespace graphic

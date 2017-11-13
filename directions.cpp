#include "directions.h"

Directions::Direction Directions::UP = Directions::Direction(0, -1);
Directions::Direction Directions::DOWN = Directions::Direction(0, 1);
Directions::Direction Directions::LEFT = Directions::Direction(-1, 0);
Directions::Direction Directions::RIGHT = Directions::Direction(1, 0);
Directions::Direction Directions::STOP = Directions::Direction(0, 0);
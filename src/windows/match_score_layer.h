#pragma once

#include <pebble.h>
#include "../match/score.h"

void match_score_layer_init(Layer* );
void match_score_layer_destroy();
void match_score_layer_draw_score(Score* );
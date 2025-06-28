//
// Created by backo on 27/05/25.
//

#include "feature.h"

void dpp_structures::feature::attach(dpp::cluster* _bot) {
    this->bot = _bot;
}

void dpp_structures::feature::setup() {}

void dpp_structures::feature::setup_all(dpp::cluster& bot) {
    for (auto& feature : feature::features) {
        feature->attach(&bot);
        feature->setup();
    }
}

dpp_structures::feature::feature() { feature::features.push_back(this); }

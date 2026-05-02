#include "overlay.h"
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

void render() {}

int main()
{
    Overlay* overlay = new Overlay(L"Consolas", 18.0);

    if (!overlay->init())
        return 1;
    if (!overlay->startup_d2d())
        return 1;

    printf("Successfully initialized, rendering for 10 seconds now..\n");
    /* you can also put this in a thread */
    for (auto start = std::chrono::steady_clock::now(), now = start; now < start + std::chrono::seconds {10}; now = std::chrono::steady_clock::now())
    {
        overlay->begin_scene();
        overlay->clear_scene();
        overlay->draw_text(10, 10, "github.com/iraizo || https://raizo.dev", D2D1::ColorF(D2D1::ColorF::Black));
        overlay->end_scene();
    }

    printf("Ran for 10 seconds, Exiting..\n");

    delete overlay;

    return 0;
}
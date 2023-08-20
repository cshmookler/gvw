#include <iostream>
#include <thread>
#include "../../gvw/gvw.hpp"
#include "../../utils/ansiec/ansiec.hpp"

glm::vec3 ColorCascadeGenerator(float Color_Scale)
{
    const float SCALE_PER_COLOR_TRANSITION = 0.5F;
    const float DENORMALIZED = Color_Scale / 0.5F;
    if (Color_Scale < SCALE_PER_COLOR_TRANSITION) {
        const float RENORMALIZED = DENORMALIZED;
        return { (1.F - RENORMALIZED), RENORMALIZED, 0.F };
    }
    const float RENORMALIZED = (DENORMALIZED - 1.F);
    return { 0.F, (1.F - RENORMALIZED), RENORMALIZED };
}

bool isCLeftOfLineAB(gvw::coordinate<int> A, // NOLINT
                     gvw::coordinate<int> B, // NOLINT
                     gvw::coordinate<int> C) // NOLINT
{
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x) > 0;
}

int main() // NOLINT
{
    // TESTING
    // std::cout << isCLeftOfLineAB({ 0, 1 }, { 1, 0 }, { 1, -1 }) << std::endl;
    // std::cout << isCLeftOfLineAB({ 0, 1 }, { 1, 0 }, { 3, -1 }) << std::endl;
    // std::cout << isCLeftOfLineAB({ 0, 0 }, { 1, 1 }, { 1, 2 }) << std::endl;
    // std::cout << isCLeftOfLineAB({ 0, 0 }, { 1, 1 }, { 2, 1 }) << std::endl;

    // std::cout << isCLeftOfLineAB({ 1, 0 }, { 0, 1 }, { -1, 1 }) << std::endl;
    // std::cout << isCLeftOfLineAB({ 1, 0 }, { 0, 1 }, { 1, -1 }) << std::endl;
    // std::cout << isCLeftOfLineAB({ 1, 1 }, { 0, 0 }, { -1, 0 }) << std::endl;
    // std::cout << isCLeftOfLineAB({ 1, 1 }, { 0, 0 }, { 0, -1 }) << std::endl;

    gvw::ptr gvw =
        gvw::Get({ .applicationInfo = { .pApplicationName = "breakout",
                                        .applicationVersion =
                                            VK_MAKE_VERSION(1, 0, 0) } });
    gvw::monitor_ptr primaryMonitor = gvw->PrimaryMonitor();

    const gvw::area<int> PRIMARY_MONITOR_SIZE = primaryMonitor->WorkAreaSize();
    const std::vector<gvw::vertex> WHITE_VERTICES = {
        { { -1.0F, -1.0F }, { 1.0F, 1.0F, 1.0F } },
        { { 1.0F, -1.0F }, { 1.0F, 1.0F, 1.0F } },
        { { -1.0F, 1.0F }, { 1.0F, 1.0F, 1.0F } },
        { { -1.0F, 1.0F }, { 1.0F, 1.0F, 1.0F } },
        { { 1.0F, -1.0F }, { 1.0F, 1.0F, 1.0F } },
        { { 1.0F, 1.0F }, { 1.0F, 1.0F, 1.0F } }
    };
    const gvw::window_creation_hints CREATION_HINTS = {
        { .resizable = GLFW_FALSE,
          .decorated = GLFW_FALSE,
          .floating = GLFW_TRUE }
    };
    const gvw::coordinate<int> BLOCK_WINDOW_COUNT = { 10, 8 };
    const int BLOCK_WINDOW_TOTAL_COUNT =
        BLOCK_WINDOW_COUNT.x * BLOCK_WINDOW_COUNT.y;
    const float BLOCK_AREA_HEIGHT =
        float(PRIMARY_MONITOR_SIZE.height) * (float(2) / float(3));
    const gvw::window_size BLOCK_WINDOW_SIZE = {
        { int(float(PRIMARY_MONITOR_SIZE.width) / float(BLOCK_WINDOW_COUNT.x)),
          int(BLOCK_AREA_HEIGHT / float(BLOCK_WINDOW_COUNT.y)) }
    };
    const gvw::area<float> PLAT_WINDOW_SIZE_TO_MONITOR_SIZE_RATIO = {
        (1.F / 8.F), (1.F / 40.F)
    };
    const gvw::window_size PLAT_WINDOW_SIZE = {
        { int(float(PRIMARY_MONITOR_SIZE.width) *
              PLAT_WINDOW_SIZE_TO_MONITOR_SIZE_RATIO.width),
          int(float(PRIMARY_MONITOR_SIZE.height) *
              PLAT_WINDOW_SIZE_TO_MONITOR_SIZE_RATIO.height) }
    };
    const gvw::coordinate<int> PLAT_WINDOW_POSITION = {
        ((PRIMARY_MONITOR_SIZE.width / 2) - (PLAT_WINDOW_SIZE->width / 2)),
        (PRIMARY_MONITOR_SIZE.height - PLAT_WINDOW_SIZE->height)
    };
    gvw::window_event_callbacks platWindowEventCallbacks =
        gvw::NO_WINDOW_EVENT_CALLBACKS;
    platWindowEventCallbacks.keyCallback = gvw::AppendToKeyEventBuffer;
    const int PLAT_HORIZONTAL_SPEED = 400;
    const gvw::window_size BALL_WINDOW_SIZE = { { PLAT_WINDOW_SIZE->height,
                                                  PLAT_WINDOW_SIZE->height } };
    const gvw::coordinate<int> BALL_WINDOW_POSITION = {
        ((PRIMARY_MONITOR_SIZE.width / 2) - (BALL_WINDOW_SIZE->width / 2)),
        (int(BLOCK_AREA_HEIGHT) +
         ((PLAT_WINDOW_POSITION.y - int(BLOCK_AREA_HEIGHT)) / 2))
    };

    std::vector<gvw::vertex> blockVertices = {
        { { -1.0F, -1.0F }, { 0.0F, 0.0F, 1.0F } },
        { { 1.0F, -1.0F }, { 1.0F, 0.0F, 0.0F } },
        { { -1.0F, 1.0F }, { 0.0F, 1.0F, 0.0F } },
        { { -1.0F, 1.0F }, { 0.0F, 1.0F, 0.0F } },
        { { 1.0F, -1.0F }, { 1.0F, 0.0F, 0.0F } },
        { { 1.0F, 1.0F }, { 0.0F, 0.0F, 1.0F } }
    };
    std::vector<gvw::window_ptr> blocks;
    blocks.resize(BLOCK_WINDOW_TOTAL_COUNT);
    for (int yIndex = 0; yIndex < BLOCK_WINDOW_COUNT.y; ++yIndex) {
        float colorCascadeScale =
            (float(yIndex + 1) / float(BLOCK_WINDOW_COUNT.y));
        for (int xIndex = 0; xIndex < BLOCK_WINDOW_COUNT.x; ++xIndex) {
            gvw::window_ptr& block =
                blocks.at((yIndex * BLOCK_WINDOW_COUNT.x) + xIndex);

            for (auto& vertex : blockVertices) {
                vertex.color = ColorCascadeGenerator(colorCascadeScale);
            }

            block = gvw->CreateWindow(
                { .position = { { BLOCK_WINDOW_SIZE->width * xIndex,
                                  BLOCK_WINDOW_SIZE->height * yIndex } },
                  .size = BLOCK_WINDOW_SIZE,
                  .title = gvw::window_title::BLANK,
                  .creationHints = CREATION_HINTS,
                  .sizeOfVerticesInBytes =
                      (sizeof(gvw::vertex) * blockVertices.size()) });
            block->DrawFrame(blockVertices);
        }
    }

    gvw::window_ptr ball =
        gvw->CreateWindow({ .position = BALL_WINDOW_POSITION,
                            .size = BALL_WINDOW_SIZE,
                            .title = gvw::window_title::BLANK,
                            .creationHints = CREATION_HINTS,
                            .sizeOfVerticesInBytes = (sizeof(gvw::vertex) *
                                                      WHITE_VERTICES.size()) });
    ball->DrawFrame(WHITE_VERTICES);

    gvw::window_ptr plat =
        gvw->CreateWindow({ .position = PLAT_WINDOW_POSITION,
                            .size = PLAT_WINDOW_SIZE,
                            .title = gvw::window_title::BLANK,
                            .creationHints = CREATION_HINTS,
                            .eventCallbacks = platWindowEventCallbacks,
                            .sizeOfVerticesInBytes = (sizeof(gvw::vertex) *
                                                      WHITE_VERTICES.size()) });
    plat->DrawFrame(WHITE_VERTICES);

    const gvw::coordinate<float> BALL_VELOCITY_INIT = { -230.F, 120.F };
    float ballVelocityModifier = 1;
    gvw::coordinate<float> ballVelocity = BALL_VELOCITY_INIT;

    std::chrono::nanoseconds deltaTime{ 0 };
    std::chrono::nanoseconds timeElapsed{ 0 };
    int fps = 0;
    const float MAX_FPS = 50;
    const int USPF = int(1000000.F / MAX_FPS);
    float spf = 0.F;
    std::chrono::time_point<std::chrono::system_clock> timeAtStartOfLastFrame =
        std::chrono::system_clock::now();

    while (!plat->ShouldClose()) {
        std::this_thread::sleep_for(std::chrono::microseconds(USPF));

        deltaTime = (std::chrono::system_clock::now() - timeAtStartOfLastFrame);
        spf = (float(deltaTime.count()) / float(1000000000)); // NOLINT
        timeElapsed += deltaTime;
        if (std::chrono::duration_cast<std::chrono::seconds>(timeElapsed) >=
            std::chrono::seconds(1)) {
            std::cout << fps << std::endl;
            fps = 0;
            timeElapsed = std::chrono::nanoseconds(0);
        }
        fps++;
        timeAtStartOfLastFrame = std::chrono::system_clock::now();

        gvw->PollEvents();

        gvw::coordinate<int> ballPosition = ball->Position();
        ballPosition.x += int(ballVelocity.x * spf);
        ballPosition.y += int(ballVelocity.y * spf);
        if (ballPosition.x < 10) {
            ballVelocity.x = (0.F - ballVelocity.x);
        }
        if (ballPosition.y < 10) {
            ballVelocity.y = (0.F - ballVelocity.y);
        }
        if (ballPosition.x + BALL_WINDOW_SIZE->width >
            (PRIMARY_MONITOR_SIZE.width - 10)) {
            ballVelocity.x = (0.F - ballVelocity.x);
        }
        if (ballPosition.y + BALL_WINDOW_SIZE->height >
            (PRIMARY_MONITOR_SIZE.height - 10)) {
            ballVelocity.y = (0.F - ballVelocity.y);
            std::cout << ansiec::BOLD << ansiec::RED_FG
                      << "GAME OVER. YOU LOSE." << ansiec::RESET << std::endl;
            plat->ShouldClose(true);
        }
        ball->Position(ballPosition);

        bool continueGame = false;
        for (gvw::window_ptr& block : blocks) {
            if (block == nullptr) {
                continue;
            }
            continueGame = true;

            gvw::coordinate<int> position = block->Position();
            gvw::area<int> size = block->Size();
            if ((((ballPosition.x + BALL_WINDOW_SIZE->width) > position.x) &&
                 (ballPosition.x < (position.x + size.width))) &&
                (((ballPosition.y + BALL_WINDOW_SIZE->height) > position.y) &&
                 (ballPosition.y < (position.y + size.height)))) {
                const gvw::coordinate<int> BALL_TOP_LEFT = ballPosition;
                const gvw::coordinate<int> BALL_TOP_RIGHT = {
                    ballPosition.x + BALL_WINDOW_SIZE->width, ballPosition.y
                };
                const gvw::coordinate<int> BALL_BOTTOM_LEFT = {
                    ballPosition.x, ballPosition.y + BALL_WINDOW_SIZE->height
                };
                const gvw::coordinate<int> BALL_BOTTOM_RIGHT = {
                    ballPosition.x + BALL_WINDOW_SIZE->width,
                    ballPosition.y + BALL_WINDOW_SIZE->height
                };
                const uint LEFT_SIDE = 1;
                const uint RIGHT_SIDE = 2;
                const uint TOP_SIDE = 4;
                const uint BOTTOM_SIDE = 8;
                uint possibleCollisionSides =
                    LEFT_SIDE | RIGHT_SIDE | TOP_SIDE | BOTTOM_SIDE;
                if (ballVelocity.x == 0) {
                    possibleCollisionSides &= TOP_SIDE | BOTTOM_SIDE;
                }
                if (ballVelocity.y == 0) {
                    possibleCollisionSides &= LEFT_SIDE | RIGHT_SIDE;
                }
                if (ballVelocity.x > 0) {
                    possibleCollisionSides &=
                        TOP_SIDE | LEFT_SIDE | BOTTOM_SIDE;
                }
                if (ballVelocity.x < 0) {
                    possibleCollisionSides &=
                        TOP_SIDE | RIGHT_SIDE | BOTTOM_SIDE;
                }
                if (ballVelocity.y > 0) {
                    possibleCollisionSides &= LEFT_SIDE | TOP_SIDE | RIGHT_SIDE;
                }
                if (ballVelocity.y < 0) {
                    possibleCollisionSides &=
                        LEFT_SIDE | BOTTOM_SIDE | RIGHT_SIDE;
                }
                switch (possibleCollisionSides) {
                    case 0:
                        break;
                    case LEFT_SIDE:
                    case RIGHT_SIDE:
                        ballVelocity.x = (0.F - ballVelocity.x);
                        break;
                    case TOP_SIDE:
                    case BOTTOM_SIDE:
                        ballVelocity.y = (0.F - ballVelocity.y);
                        break;
                    case LEFT_SIDE | TOP_SIDE:
                        if (isCLeftOfLineAB(
                                BALL_TOP_LEFT, BALL_BOTTOM_RIGHT, position)) {
                            ballVelocity.y = (0.F - ballVelocity.y);
                        } else {
                            ballVelocity.x = (0.F - ballVelocity.x);
                        }
                        break;
                    case TOP_SIDE | RIGHT_SIDE:
                        if (isCLeftOfLineAB(
                                BALL_TOP_RIGHT,
                                BALL_BOTTOM_LEFT,
                                { position.x + BLOCK_WINDOW_SIZE->width,
                                  position.y })) {
                            ballVelocity.y = (0.F - ballVelocity.y);
                        } else {
                            ballVelocity.x = (0.F - ballVelocity.x);
                        }
                        break;
                    case RIGHT_SIDE | BOTTOM_SIDE:
                        if (isCLeftOfLineAB(
                                BALL_BOTTOM_RIGHT,
                                BALL_TOP_LEFT,
                                { position.x + BLOCK_WINDOW_SIZE->width,
                                  position.y + BLOCK_WINDOW_SIZE->height })) {
                            ballVelocity.y = (0.F - ballVelocity.y);
                        } else {
                            ballVelocity.x = (0.F - ballVelocity.x);
                        }
                        break;
                    case BOTTOM_SIDE | LEFT_SIDE:
                        if (isCLeftOfLineAB(
                                BALL_BOTTOM_LEFT,
                                BALL_TOP_RIGHT,
                                { position.x,
                                  position.y + BLOCK_WINDOW_SIZE->height })) {
                            ballVelocity.x = (0.F - ballVelocity.x);
                        } else {
                            ballVelocity.y = (0.F - ballVelocity.y);
                        }
                        break;
                    default:
                        throw std::runtime_error(
                            "Variable 'possibleCollisionSides' is an invalid "
                            "value: " +
                            std::to_string(possibleCollisionSides));
                        break;
                }
                const auto BALL_VELOCITY_MODIFIER_NEW =
                    1.F +
                    ((float(BLOCK_WINDOW_COUNT.y) -
                      (float(position.y) / float(BLOCK_WINDOW_SIZE->height))) /
                     (float(BLOCK_WINDOW_COUNT.y) / 2.F));
                std::cout << "M: " << BALL_VELOCITY_MODIFIER_NEW << std::endl;
                ballVelocity.x = (ballVelocity.x / ballVelocityModifier) *
                                 BALL_VELOCITY_MODIFIER_NEW;
                ballVelocity.y = (ballVelocity.y / ballVelocityModifier) *
                                 BALL_VELOCITY_MODIFIER_NEW;
                ballVelocityModifier = BALL_VELOCITY_MODIFIER_NEW;
                block.reset();
            }
        }
        if (!continueGame) {
            std::cout << ansiec::BOLD << ansiec::GREEN_FG << "YOU WIN!"
                      << ansiec::RESET << std::endl;
            plat->ShouldClose(true);
        }

        if (plat->KeyState(GLFW_KEY_A) == GLFW_PRESS) {
            auto position = plat->Position();
            position.x -= int(float(PLAT_HORIZONTAL_SPEED) * spf);
            plat->Position(position);
        }
        if (plat->KeyState(GLFW_KEY_D) == GLFW_PRESS) {
            auto position = plat->Position();
            position.x += int(float(PLAT_HORIZONTAL_SPEED) * spf);
            plat->Position(position);
        }
        plat->ClearEvents();

        auto platPosition = plat->Position();
        if (((ballPosition.x + BALL_WINDOW_SIZE->width) > platPosition.x) &&
            (ballPosition.x < (platPosition.x + PLAT_WINDOW_SIZE->width)) &&
            ((ballPosition.y + BALL_WINDOW_SIZE->height) > platPosition.y)) {
            ballVelocity.y = (0.F - ballVelocity.y);
        }
    }
}
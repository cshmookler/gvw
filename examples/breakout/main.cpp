#include <iostream>
#include <thread>
#include <algorithm>
#include <random>
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
    // vertex = { x, y, z }
    // color = { r, g, b }
    // colored_vertex = {{ x, y, z }, { r, g, b }}
    // index = uint16_t

    // TESTING
    // std::cout << isCLeftOfLineAB({ 0, 1 }, { 1, 0 }, { 1, -1 }) << std::endl;
    // std::cout << isCLeftOfLineAB({ 0, 1 }, { 1, 0 }, { 3, -1 }) << std::endl;
    // std::cout << isCLeftOfLineAB({ 0, 0 }, { 1, 1 }, { 1, 2 }) << std::endl;
    // std::cout << isCLeftOfLineAB({ 0, 0 }, { 1, 1 }, { 2, 1 }) << std::endl;

    // std::cout << isCLeftOfLineAB({ 1, 0 }, { 0, 1 }, { -1, 1 }) << std::endl;
    // std::cout << isCLeftOfLineAB({ 1, 0 }, { 0, 1 }, { 1, -1 }) << std::endl;
    // std::cout << isCLeftOfLineAB({ 1, 1 }, { 0, 0 }, { -1, 0 }) << std::endl;
    // std::cout << isCLeftOfLineAB({ 1, 1 }, { 0, 0 }, { 0, -1 }) << std::endl;

    gvw::instance_ptr gvw = gvw::CreateInstance(
        { .applicationInfo = { .pApplicationName = "breakout",
                               .applicationVersion =
                                   VK_MAKE_VERSION(1, 0, 0) } });
    gvw::monitor_ptr primaryMonitor = gvw->GetPrimaryMonitor();

    const gvw::area<int> PRIMARY_MONITOR_SIZE =
        primaryMonitor->GetWorkAreaSize();
    const std::vector<gvw::xy_rgb> WHITE_VERTICES = {
        { { -1.0F, -1.0F }, { 1.0F, 1.0F, 1.0F } },
        { { 1.0F, -1.0F }, { 1.0F, 1.0F, 1.0F } },
        { { -1.0F, 1.0F }, { 1.0F, 1.0F, 1.0F } },
        { { -1.0F, 1.0F }, { 1.0F, 1.0F, 1.0F } },
        { { 1.0F, -1.0F }, { 1.0F, 1.0F, 1.0F } },
        { { 1.0F, 1.0F }, { 1.0F, 1.0F, 1.0F } }
    };
    const gvw::window_creation_hints CREATION_HINTS = {
        { .resizable = false, .decorated = false, .floating = true }
    };
    const gvw::coordinate<int> BLOCK_WINDOW_COUNT = { 8, 8 };
    const int BLOCK_WINDOW_TOTAL_COUNT =
        BLOCK_WINDOW_COUNT.x * BLOCK_WINDOW_COUNT.y;
    const float BLOCK_AREA_HEIGHT =
        float(PRIMARY_MONITOR_SIZE.height) * (float(2) / float(3));
    const gvw::window_size BLOCK_WINDOW_SIZE = {
        int(float(PRIMARY_MONITOR_SIZE.width) / float(BLOCK_WINDOW_COUNT.x)),
        int(BLOCK_AREA_HEIGHT / float(BLOCK_WINDOW_COUNT.y))
    };
    const gvw::area<float> PLAT_WINDOW_SIZE_TO_MONITOR_SIZE_RATIO = {
        (1.F / 8.F), (1.F / 40.F)
    };
    const gvw::window_size PLAT_WINDOW_SIZE = {
        int(float(PRIMARY_MONITOR_SIZE.width) *
            PLAT_WINDOW_SIZE_TO_MONITOR_SIZE_RATIO.width),
        int(float(PRIMARY_MONITOR_SIZE.height) *
            PLAT_WINDOW_SIZE_TO_MONITOR_SIZE_RATIO.height)
    };
    const gvw::coordinate<int> PLAT_WINDOW_POSITION = {
        ((PRIMARY_MONITOR_SIZE.width / 2) - (PLAT_WINDOW_SIZE.width / 2)),
        (PRIMARY_MONITOR_SIZE.height - PLAT_WINDOW_SIZE.height)
    };
    gvw::window_event_callbacks platWindowEventCallbacks = {
        .keyCallback =
            gvw::window_key_event_callback_config::APPEND_TO_KEY_EVENT_BUFFER
    };

    const int PLAT_HORIZONTAL_SPEED = 800;
    const gvw::window_size BALL_WINDOW_SIZE = { PLAT_WINDOW_SIZE.height,
                                                PLAT_WINDOW_SIZE.height };
    const gvw::coordinate<int> BALL_WINDOW_POSITION = {
        ((PRIMARY_MONITOR_SIZE.width / 2) - (BALL_WINDOW_SIZE.width / 2)),
        (int(BLOCK_AREA_HEIGHT) +
         ((PLAT_WINDOW_POSITION.y - int(BLOCK_AREA_HEIGHT)) / 2))
    };

    std::vector<gvw::xy_rgb> blockVertices = {
        { { -1.0F, -1.0F }, { 0.0F, 0.0F, 1.0F } },
        { { 1.0F, -1.0F }, { 1.0F, 0.0F, 0.0F } },
        { { -1.0F, 1.0F }, { 0.0F, 1.0F, 0.0F } },
        { { -1.0F, 1.0F }, { 0.0F, 1.0F, 0.0F } },
        { { 1.0F, -1.0F }, { 1.0F, 0.0F, 0.0F } },
        { { 1.0F, 1.0F }, { 0.0F, 0.0F, 1.0F } }
    };

    gvw::window_ptr plat = gvw->CreateWindow(
        { .position = PLAT_WINDOW_POSITION,
          .size = PLAT_WINDOW_SIZE,
          .title = gvw::window_title_config::BLANK,
          .creationHints = CREATION_HINTS,
          .eventCallbacks = platWindowEventCallbacks,
          .staticVertices = WHITE_VERTICES,
          .sizeOfDynamicDataVerticesInBytes =
              (sizeof(gvw::xy_rgb) * WHITE_VERTICES.size()) });
    plat->DrawFrame(WHITE_VERTICES);

    gvw::image_file_info imageInfo = { .path = "pointer.png" };
    auto cursorImage = gvw::CreateImage(imageInfo);
    gvw::image_file_info otherImageInfo = { .path = "testing.png" };
    auto otherImage = gvw::CreateImage(otherImageInfo);
    gvw::cursor_custom_shape_info cursorInfo = {
        .image = cursorImage, .hotspot = cursorImage->GetSize() / 2
    };
    auto cursor = gvw->CreateCursor(cursorInfo);
    plat->SetCursorShape(cursor);
    plat->SetIcon(cursorImage);

    std::vector<std::pair<gvw::coordinate<int>, float>> blockCreationInfos;
    blockCreationInfos.reserve(BLOCK_WINDOW_TOTAL_COUNT);
    for (int yIndex = 0; yIndex < BLOCK_WINDOW_COUNT.y; ++yIndex) {
        float colorCascadeScale =
            (float(yIndex + 1) / float(BLOCK_WINDOW_COUNT.y));
        for (int xIndex = 0; xIndex < BLOCK_WINDOW_COUNT.x; ++xIndex) {
            blockCreationInfos.emplace_back(
                gvw::coordinate<int>{ BLOCK_WINDOW_SIZE.width * xIndex,
                                      BLOCK_WINDOW_SIZE.height * yIndex },
                float{ colorCascadeScale });
        }
    }

    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator(randomDevice());
    std::shuffle(blockCreationInfos.begin(),
                 blockCreationInfos.end(),
                 randomNumberGenerator);

    std::vector<gvw::window_ptr> blocks;
    blocks.reserve(BLOCK_WINDOW_TOTAL_COUNT);
    for (const auto& blockCreationInfo : blockCreationInfos) {
        for (auto& vertex : blockVertices) {
            vertex.second = ColorCascadeGenerator(blockCreationInfo.second);
        }

        blocks.emplace_back(plat->CreateChildWindow(
            { .position = blockCreationInfo.first,
              .size = BLOCK_WINDOW_SIZE,
              .title = gvw::window_title_config::BLANK,
              .creationHints = CREATION_HINTS,
              .staticVertices = blockVertices,
              .sizeOfDynamicDataVerticesInBytes =
                  (sizeof(gvw::xy_rgb) * blockVertices.size()) }));
        blocks.back()->DrawFrame(blockVertices);
        blocks.back()->SetCursorShape(cursor);
        blocks.back()->SetIcon(cursorImage);
    }

    gvw::window_ptr ball = plat->CreateChildWindow(
        { .position = BALL_WINDOW_POSITION,
          .size = BALL_WINDOW_SIZE,
          .title = gvw::window_title_config::BLANK,
          .creationHints = CREATION_HINTS,
          .staticVertices = WHITE_VERTICES,
          .sizeOfDynamicDataVerticesInBytes =
              (sizeof(gvw::xy_rgb) * WHITE_VERTICES.size()) });
    ball->DrawFrame(WHITE_VERTICES);

    plat->Focus();

    const gvw::coordinate<float> BALL_VELOCITY_INIT = { -430.F, 320.F };
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

        gvw::coordinate<int> ballPosition = ball->GetPosition();
        ballPosition.x += int(ballVelocity.x * spf);
        ballPosition.y += int(ballVelocity.y * spf);
        if (ballPosition.x < 10) {
            ballVelocity.x = (0.F - ballVelocity.x);
        }
        if (ballPosition.y < 10) {
            ballVelocity.y = (0.F - ballVelocity.y);
        }
        if (ballPosition.x + BALL_WINDOW_SIZE.width >
            (PRIMARY_MONITOR_SIZE.width - 10)) {
            ballVelocity.x = (0.F - ballVelocity.x);
        }
        if (ballPosition.y + BALL_WINDOW_SIZE.height >
            (PRIMARY_MONITOR_SIZE.height - 10)) {
            ballVelocity.y = (0.F - ballVelocity.y);
            // std::cout << ansiec::BOLD << ansiec::RAPID_BLINK <<
            // ansiec::RED_FG
            //           << "\nGAME OVER. YOU LOSE.\n"
            //           << ansiec::RESET << std::endl;
            // plat->ShouldClose(true);
        }
        ball->SetPosition(ballPosition);

        bool continueGame = false;
        for (gvw::window_ptr& block : blocks) {
            if (block == nullptr) {
                continue;
            }
            continueGame = true;

            gvw::coordinate<int> position = block->GetPosition();
            gvw::area<int> size = block->GetSize();
            if ((((ballPosition.x + BALL_WINDOW_SIZE.width) > position.x) &&
                 (ballPosition.x < (position.x + size.width))) &&
                (((ballPosition.y + BALL_WINDOW_SIZE.height) > position.y) &&
                 (ballPosition.y < (position.y + size.height)))) {
                const gvw::coordinate<int> BALL_TOP_LEFT = ballPosition;
                const gvw::coordinate<int> BALL_TOP_RIGHT = {
                    ballPosition.x + BALL_WINDOW_SIZE.width, ballPosition.y
                };
                const gvw::coordinate<int> BALL_BOTTOM_LEFT = {
                    ballPosition.x, ballPosition.y + BALL_WINDOW_SIZE.height
                };
                const gvw::coordinate<int> BALL_BOTTOM_RIGHT = {
                    ballPosition.x + BALL_WINDOW_SIZE.width,
                    ballPosition.y + BALL_WINDOW_SIZE.height
                };
                const unsigned int LEFT_SIDE = 1;
                const unsigned int RIGHT_SIDE = 2;
                const unsigned int TOP_SIDE = 4;
                const unsigned int BOTTOM_SIDE = 8;
                unsigned int possibleCollisionSides =
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
                                { position.x + BLOCK_WINDOW_SIZE.width,
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
                                { position.x + BLOCK_WINDOW_SIZE.width,
                                  position.y + BLOCK_WINDOW_SIZE.height })) {
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
                                  position.y + BLOCK_WINDOW_SIZE.height })) {
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
                      (float(position.y) / float(BLOCK_WINDOW_SIZE.height))) /
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
            std::cout << ansiec::BOLD << ansiec::RAPID_BLINK << ansiec::GREEN_FG
                      << "\nYOU WIN!\n"
                      << ansiec::RESET << std::endl;
            plat->ShouldClose(true);
        }

        if (plat->GetKeyState(gvw::window_key::eA) ==
            gvw::window_key_action::ePress) {
            auto position = plat->GetPosition();
            position.x -= int(float(PLAT_HORIZONTAL_SPEED) * spf);
            plat->SetPosition(position);
        }
        if (plat->GetKeyState(gvw::window_key::eD) ==
            gvw::window_key_action::ePress) {
            auto position = plat->GetPosition();
            position.x += int(float(PLAT_HORIZONTAL_SPEED) * spf);
            plat->SetPosition(position);
        }
        plat->ClearEvents();

        auto platPosition = plat->GetPosition();
        if (((ballPosition.x + BALL_WINDOW_SIZE.width) > platPosition.x) &&
            (ballPosition.x < (platPosition.x + PLAT_WINDOW_SIZE.width)) &&
            ((ballPosition.y + BALL_WINDOW_SIZE.height) > platPosition.y)) {
            ballVelocity.y = (0.F - ballVelocity.y);
        }
    }
}
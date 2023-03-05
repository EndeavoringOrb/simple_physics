#include <iostream>
#include <iomanip>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <string>
#include <chrono>
#include <thread>
#include <random>
#include <deque>

int multiplier = 1;
float gravitational_constant = 9.81f*multiplier;
float initial_velocity = 1;
float fps_threshold = 60.0f;
float gravity_fps = 480;
int ball_radius = 5;
int balls = 500;
int font_size = 12;
int fps_averaging_length = 10;
int substeps = 8;
float elasticity_mult = 0.998;
float pi = 3.141592653589793;

static sf::Color getRainbow(float t)
{
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * pi);
    const float b = sin(t + 0.66f * 2.0f * pi);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

int main()
{
    // Set up SFML window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Circle Collision");

    // Generate circles
    std::vector<sf::CircleShape> circles;
    std::vector<sf::Vector2f> velocities;

    sf::Font font;
    if (!font.loadFromFile("OpenSans-Regular.ttf")) {}

    sf::Text text1;
    text1.setString("Hello, world!");
    text1.setFont(font);  // replace 'font' with your font object
    text1.setCharacterSize(font_size);
    text1.setFillColor(sf::Color::White);
    text1.setPosition(0.0f, 0.0f);

    sf::Text text2;
    text2.setString("Hello, world!");
    text2.setFont(font);  // replace 'font' with your font object
    text2.setCharacterSize(font_size);
    text2.setFillColor(sf::Color::White);
    text2.setPosition(0.0f, font_size);

    sf::Text text3;
    text3.setString("Hello, world!");
    text3.setFont(font);  // replace 'font' with your font object
    text3.setCharacterSize(font_size);
    text3.setFillColor(sf::Color::White);
    text3.setPosition(0.0f, font_size*2);

    sf::Text text4;
    text4.setString("Hello, world!");
    text4.setFont(font);  // replace 'font' with your font object
    text4.setCharacterSize(12);
    text4.setFillColor(sf::Color::White);
    text4.setPosition(0.0f, font_size*3);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-pi/2, pi/2);

    float fps = fps_threshold + 1;
    int no_more_balls = 0;
    int collision = 1;
    int count = 0;
    std::deque<float> fps_arr;
    float avg_fps = fps;


    std::stringstream ss; // create a stringstream object

    // Main loop
    auto program_start_time = std::chrono::high_resolution_clock::now();
    while (window.isOpen())
    {   
        auto start_time = std::chrono::high_resolution_clock::now();
        // Event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
        }

        // Update circle positions
        float deltaTime = 1.0f / gravity_fps;

        if (no_more_balls != 1) {
            // Spawn new circle
            sf::CircleShape circle(ball_radius);
            circle.setPosition(sf::Vector2f(10, 50));
            circle.setFillColor(getRainbow(dis(gen)));
            circles.push_back(circle);
            velocities.push_back(sf::Vector2f(initial_velocity,0.0f));
        }

        // Detect collisions
        collision = 1;
        //count = 0;
        for (int i = 0; i < substeps; i++) {
            // GRAVITY + RANDOM UP FORCE
            for (int i = 0; i < circles.size(); i++)
            {
                // Apply gravity
                if (rand() % 10 > 1) {
                    velocities[i] += sf::Vector2f(0.0f, gravitational_constant * deltaTime / substeps);
                    circles[i].move(velocities[i]);
                }
            }
            // OBJECT COLLISION
            collision = 0;
            for (int i = 0; i < circles.size(); i++)
            {
                for (int j = 0; j < circles.size(); j++)
                {
                    // Resolve collision
                    if (j != i) {
                        sf::Vector2f normal = sf::Vector2f(circles[j].getPosition().x - circles[i].getPosition().x, circles[j].getPosition().y - circles[i].getPosition().y);
                        float distance = std::sqrt(normal.x * normal.x + normal.y * normal.y);
                        if (distance < (ball_radius*2)) {
                            count += 1;
                            ss.str("");
                            ss << count << " collisions";
                            text4.setString(ss.str());
                            collision = 1;
                            if (distance == 0) {
                                distance = 0.001f;
                            }
                            float overlap = ball_radius*2 - distance;
                            overlap /= substeps;
                            normal /= distance;
                            //
                            float iratio = 0.5f; //(((ball_radius*2)-distance)/(2*ball_radius*2))
                            float jratio = 0.5f;
                            circles[i].setPosition(circles[i].getPosition() - sf::Vector2f(((overlap)/2) * normal.x, ((overlap)/2) * normal.y));
                            circles[j].setPosition(circles[j].getPosition() + sf::Vector2f(((overlap)/2) * normal.x, ((overlap)/2) * normal.y));
                            /*if (velocities[i] == sf::Vector2f(0,0)) {
                                velocities[i] = sf::Vector2f(0,0);
                                velocities[j] = sf::Vector2f(0,0);
                            }
                            else if (velocities[j] == sf::Vector2f(0,0)) {
                                velocities[i] = sf::Vector2f(0,0);
                                velocities[j] = sf::Vector2f(0,0);
                            }
                            else {
                            velocities[i] = sf::Vector2f((velocities[j].x + velocities[i].x)/2, (velocities[j].y + velocities[i].y)/2);
                            velocities[j] = sf::Vector2f((velocities[j].x + velocity_intermed.x)/2, (velocities[j].y + velocity_intermed.y)/2);
                            }*/
                            sf::Vector2f velocity_intermed = velocities[i];
                            velocities[i] = velocities[j] * elasticity_mult;
                            velocities[j] = velocity_intermed * elasticity_mult;
                        }
                    }
                }
            }
            // BORDER COLLISION
            for (int i = 0; i < circles.size(); i++)
            {
                // Check for collision with window edges
                sf::Vector2f pos = circles[i].getPosition();
                if ((pos.x) < 0)
                {
                    circles[i].setPosition(0.0f, circles[i].getPosition().y);
                    velocities[i].x = -velocities[i].x * elasticity_mult;
                }
                else if ((pos.x + ball_radius*2) > window.getSize().x)
                {
                    circles[i].setPosition(window.getSize().x - ball_radius*2, circles[i].getPosition().y);
                    velocities[i].x = -velocities[i].x * elasticity_mult;
                }
                if ((pos.y) < 0)
                {
                    circles[i].setPosition(circles[i].getPosition().x, 0.0f);
                    velocities[i].y = 0;
                }
                else if ((pos.y + ball_radius*2) > window.getSize().y)
                {
                    circles[i].setPosition(circles[i].getPosition().x, window.getSize().y - ball_radius*2);
                    velocities[i].y = 0;
                }
            }
        }

        

        // Draw circles
        window.clear();
        for (auto& circle : circles)
        {
            window.draw(circle);
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        auto total_elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - program_start_time);
        std::stringstream ss; // create a stringstream object
        text1.setString(std::to_string(circles.size()) + " balls");
        window.draw(text1);
        ss.str("");
        ss << std::fixed << std::setprecision(2) << total_elapsed_time.count() / 1000000.0f << "s elapsed";
        text2.setString(ss.str());
        window.draw(text2);
        ss.str("");
        float elapsed_time_ms = elapsed_time.count() / 1000.0f;
        float fps = 1 / (elapsed_time_ms / 1000.0f);
        float formatted_fps = std::floor(fps * 100) / 100; // Rounds to two decimal places
        fps_arr.push_back(formatted_fps);
        if (fps_arr.size() > fps_averaging_length) {
            fps_arr.pop_front();
        }
        avg_fps = 0;
        for (float num : fps_arr) {
            avg_fps += num;
        }
        avg_fps /= fps_arr.size();
        //gravity_fps = avg_fps;
        ss.str("");
        ss << avg_fps << " fps";
        text3.setString(ss.str());
        window.draw(text3);
        if (circles.size() == balls) { // avg_fps < fps_threshold
            no_more_balls = 1;
        }
        window.draw(text4);
        window.display();
        //
    }

    return 0;
}

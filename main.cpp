#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstdlib>

const unsigned int height = 800, width = 800, amount = 10;
sf::Vector2f center = {width / 2.f, height / 2.f};
sf::CircleShape boundary(300, 1000);

// I swear this is like the hundredth time I'm attempting this
struct Circle {
  sf::CircleShape shape;
  sf::Vector2f gravityVelocity, gravityAcceleration = {0.f, 0.10};

  Circle(float r) {
    float d = (static_cast<double>(rand()) / RAND_MAX) *
              (boundary.getRadius() - r - 5.f);
    float angle = (static_cast<double>(rand()) / RAND_MAX) *
                  (2.0 * 3.14159265358979323846);
    shape.setRadius(r);
    shape.setOrigin({r, r});
    shape.setPosition(
        {center.x + d * std::cos(angle), center.y + d * std::sin(angle)});
    shape.setFillColor({(uint8_t)(rand() % 255), (uint8_t)(rand() % 255),
                        (uint8_t)(rand() % 255)});
    gravityVelocity = {float(rand() % 10), float(rand() % 10)};
  }

  void collide(Circle &b) {
    sf::Vector2f n = shape.getPosition() - b.shape.getPosition();
    if (n.lengthSquared() >=
        std::pow(shape.getRadius() + b.shape.getRadius(), 2))
      return;
    if (n.lengthSquared() < 0.0001f)
      n = {1.f, 0.f};
    else
      n = n.normalized();
    float dot = (gravityVelocity - b.gravityVelocity).dot(n);
    if (dot >= 0)
      return;
    gravityVelocity -= dot * n;
    b.gravityVelocity += dot * n;
  }

  void gravity() {
    sf::Vector2f prevPos = shape.getPosition();
    gravityVelocity += gravityAcceleration;
    shape.move(gravityVelocity);
    sf::Vector2f normal = shape.getPosition() - center;
    float radius = boundary.getRadius() - shape.getRadius();
    if (normal.lengthSquared() >= radius * radius) {
      normal = normal.normalized();
      gravityVelocity -= 2.f * normal * gravityVelocity.dot(normal);
      shape.setPosition(prevPos);
    }
  }
};

int main() {
  sf::ContextSettings settings;
  settings.antiAliasingLevel = 16;
  sf::RenderWindow window(sf::VideoMode({width, height}), "My window",
                          sf::Style::Titlebar | sf::Style::Close,
                          sf::State::Windowed, settings);
  window.setFramerateLimit(60);

  sf::View view(sf::FloatRect(
      {0.f, 0.f}, {static_cast<float>(width), static_cast<float>(height)}));
  window.setView(view);
  window.setMinimumSize(sf::Vector2u{width, height});
  window.setMaximumSize(sf::Vector2u{width, height});

  boundary.setFillColor(sf::Color::Black);
  boundary.setOutlineColor(sf::Color::White);
  boundary.setOutlineThickness(3.f);
  boundary.setOrigin({boundary.getRadius(), boundary.getRadius()});
  boundary.setPosition(center);

  std::vector<Circle> circles;
  for (int i = 0; i < amount; i++)
    circles.emplace_back(rand() % 25 + 10);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent())
      if (event->is<sf::Event::Closed>())
        window.close();

    window.clear(sf::Color{0, 2, 46});
    window.draw(boundary);

    for (int i = 0; i < circles.size(); i++) {
      for (int j = i + 1; j < circles.size(); j++)
        circles[i].collide(circles[j]);
      circles[i].gravity();
      window.draw(circles[i].shape);
    }
    window.display();
  }
}
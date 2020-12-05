#include "KmeansCode.hpp"

int main() {
  vector<DataPoint> dataset;
  string op; cout << "Generate or Load?: "; cin >> op;
  if(op == "generate")
    get_data(dataset);
  else
    get_data(dataset,0);
  size_t k; cout << "N" << char(167) << " clusters: "; cin >> k;
  k_means_clustering(dataset, k);
  RenderWindow win(VideoMode(W, H), "K-Means Visualizador");
  win.setFramerateLimit(60);//FPS
  View screen(FloatRect(0.f, 0.f, W, H));
  screen.setCenter(dataset[0].longitude, dataset[0].latitude);
  Event e;
  while (win.isOpen()) {
    while (win.pollEvent(e)) {
      if (e.type == Event::Closed) { win.close(); }
      if (e.type == Event::MouseWheelMoved) { screen.zoom(1.f + e.mouseWheel.delta * 0.05f); }
      if (e.type == Event::KeyPressed) {
        if (Keyboard::isKeyPressed(Keyboard::Space))show_clusters = !show_clusters;
      }
    }
    if (Keyboard::isKeyPressed(Keyboard::Right))screen.move(screen.getSize().x /100.f, 0.f);
    if (Keyboard::isKeyPressed(Keyboard::Left))screen.move(-screen.getSize().x/100.f, 0.f);
    if (Keyboard::isKeyPressed(Keyboard::Up))screen.move(0.f, -screen.getSize().y/100.f);
    if (Keyboard::isKeyPressed(Keyboard::Down))screen.move(0.f, screen.getSize().y/100.f);
    win.setView(screen);
    win.clear(Color(255, 255, 255));//RGB
    for (DataPoint &p : dataset) {
      p.show(&win);
    }
    win.display();
  }
  return 0;
}

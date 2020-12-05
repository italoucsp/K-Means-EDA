#include "KmeansCode.hpp"

int main() {
  vector<DataPoint> dataset;
  get_data(dataset);
  size_t k; cout << "Number of clusters: "; cin >> k;
  k_means_clustering(dataset, k);
  RenderWindow win(VideoMode(W, H), "K-Means Visualizador");
  win.setFramerateLimit(60);//FPS
  View screen(FloatRect(0.f, 0.f, W, H));
  Event e;
  while (win.isOpen()) {
    while (win.pollEvent(e)) {
      if (e.type == Event::Closed) { win.close(); }
      if (e.type == Event::MouseWheelMoved) { screen.zoom(1.f + e.mouseWheel.delta * 0.05f); }
      if (e.type == Event::KeyPressed) {
        if (Keyboard::isKeyPressed(Keyboard::Space))show_clusters = !show_clusters;
      }
    }
    if (Keyboard::isKeyPressed(Keyboard::Right))screen.move(10.f, 0.f);
    if (Keyboard::isKeyPressed(Keyboard::Left))screen.move(-10.f, 0.f);    
    if (Keyboard::isKeyPressed(Keyboard::Up))screen.move(0.f, -10.f);    
    if (Keyboard::isKeyPressed(Keyboard::Down))screen.move(0.f, 10.f);
    win.setView(screen);
    win.clear(Color(255, 255, 255));//RGB
    for (DataPoint &p : dataset) {
      p.show(&win);
    }
    win.display();
  }
  return 0;
}
#include "KmeansCode.hpp"

void draw_data(vector<DataPoint> dataset, RenderWindow &win, View &screen) {
  size_t lim = 0;
  float new_radius = abs(screen.getSize().x + screen.getSize().y)/1000;
  int limit = 100000 - int(abs(screen.getSize().x + screen.getSize().y))*100;
  limit = (limit < 0) ? 0 : limit;
  limit = (limit > 100000) ? 100000 : limit;
  for (DataPoint &p : dataset) {
    p.resize(new_radius);
    if (lim == limit) { break; }
    p.show(&win);
    ++lim;
  }
}

int main() {
  vector<DataPoint> dataset;
  string op; cout << "Generate or Load?: "; cin >> op;
  if(op == "generate")
    get_data(dataset);
  else if(op == "load")
    get_data(dataset,0);
  else {
    ALERT("Command line error or not found.");
    exit(1);
  }
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
    if (show_clusters) {
      win.clear(Color(0, 0, 0));//RGB
    }
    else {
      win.clear(Color(255, 255, 255));//RGB
    }
    draw_data(dataset, win, screen);
    win.display();
  }
  return 0;
}

#include "Scene.hpp"

void input_reset_buttons();

namespace Scene
{
  unsigned long ms = 0;
  bool close = false;
  int  status = 0;
}

unsigned long Scene::timer()
{
  return ms;
}

void Scene::set_timer(unsigned long time)
{
  ms = time;
}

bool Scene::is_closed()
{
  return close;
}

void Scene::closing()
{
  close = true;
}

bool Scene::hit_close_button()
{
  return status == 1;
}

void Scene::set_close_button()
{
  status = 1;
}

bool Scene::has_crashed()
{
  return status == 2;
}

void Scene::set_crash()
{
  status = 2;
}

bool Scene::is_zombie()
{
  return status == 3;
}

void Scene::set_zombie()
{
  status = 3;
}

bool Scene::is_game_reset()
{
  return status == 4;
}

void Scene::reset_game()
{
  status = 4;
}

void Scene::clear_status()
{
  status = 0;
  input_reset_buttons();
}
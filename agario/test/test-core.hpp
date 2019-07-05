#pragma once

#include <gtest/gtest.h>
#include <engine/Engine.hpp>
#include <test/renderable.hpp>

namespace {

  TEST(Cell, Construct) {
    agario::distance x = 100;
    agario::distance y = 125;
    agario::mass mass = 25;

    agario::Cell<renderable> cell(x, y, mass);
    EXPECT_EQ(cell.x, x);
    EXPECT_EQ(cell.y, y);
    EXPECT_EQ(cell.mass(), mass);
  }

  TEST(Cell, PositionMass) {
    agario::distance x = 100;
    agario::distance y = 125;
    agario::mass mass = 25;

    agario::Cell<renderable> cell(x, y, mass);
    EXPECT_EQ(cell.x, x);
    EXPECT_EQ(cell.y, y);
    EXPECT_EQ(cell.mass(), mass);
  }

  TEST(Player, ConstructNoPid) {
    std::string name = "TestName";
    agario::Player<renderable> player(name);
    EXPECT_EQ(player.name(), name);
    EXPECT_EQ(player.pid(), agario::pid(-1));
  }

  TEST(Player, Construct) {
    agario::pid pid = 120;
    std::string name = "TestPlayer";
    agario::Player<renderable> player(pid, name);

    EXPECT_EQ(player.pid(), pid) << "Player pid did not match";
    EXPECT_EQ(player.name(), name) << "Player name incorrect";

    EXPECT_TRUE(player.dead()) << "Player did not start dead";
    EXPECT_EQ(player.cells.size(), 0ul) << "Player constructed with cells";
  }

  TEST(Player, ConstructColor) {
    agario::pid pid = 120;
    std::string name = "TestPlayer";
    agario::color color = agario::color::yellow;
    agario::Player<renderable> player(pid, name, color);

    EXPECT_EQ(player.pid(), pid) << "Player pid did not match";
    EXPECT_EQ(player.name(), name) << "Player name incorrect";
    EXPECT_EQ(player.color(), color) << "Player color did not match";
  }

  TEST(Player, StartsDead) {
    agario::Player<renderable> player(0, "TestPlayer");
    EXPECT_TRUE(player.dead());
    EXPECT_EQ(player.cells.size(), 0ul);
  }

  TEST(Player, AddCell) {
    agario::Player<renderable> player(0, "TestPlayer");

    agario::distance x = 100;
    agario::distance y = 125;
    agario::mass mass = 25;

    player.add_cell(x, y, mass);
    ASSERT_EQ(player.cells.size(), 1ul);

    EXPECT_EQ(player.cells.front().mass(), mass) << "Cell mass incorrect";
    EXPECT_EQ(player.cells.front().x, x) << "Cell x position incorrect";
    EXPECT_EQ(player.cells.front().y, y) << "Cell y position incorrect";
  }

  TEST(Player, Kill) {
    agario::Player<renderable> player(0, "TestPlayer");
    ASSERT_TRUE(player.dead());
    player.add_cell(agario::Location(0, 0), 0);
    ASSERT_FALSE(player.dead());
    player.kill();

    EXPECT_TRUE(player.dead()) << "Player not dead after being killed";
  }

  TEST(Player, SimpleLocation) {
    agario::Player<renderable> player(0, "TestPlayer");

    EXPECT_EQ(player.cells.size(), 0ul);

    agario::distance x = 100;
    agario::distance y = 100;
    agario::mass mass = 25;

    player.add_cell(x, y, mass);
    EXPECT_EQ(player.x(), x) << "Player x location incorrect";
    EXPECT_EQ(player.y(), y) << "Player y location incorrect";
  }

  TEST(Player, Location) {
    agario::Player<renderable> player(0, "TestPlayer");

    ASSERT_EQ(player.cells.size(), 0ul);

    agario::distance x = 100;
    agario::distance y = 100;
    agario::mass mass = 25;

    player.add_cell(x, y, mass);
    player.add_cell(x + 2, y + 2, mass);

    EXPECT_EQ(player.x(), x + 1) << "Player x position incorrect";
    EXPECT_EQ(player.y(), y + 1) << "Player y position incorrect";
  }

}

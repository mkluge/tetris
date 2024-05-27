#ifndef TETRISPIECE_H
#define TETRISPIECE_H

#include <Animation.h>
#include <Basics.h>
#include <list>

#pragma once

class TetrisPiece : public Animation {
public:
  TetrisPiece(LEDDisplay &display);
  ~TetrisPiece();
  int removeFullLines();
  void removeLine(int line);

  const std::list<PixelList> pieceTemplates = {
      // yellow rectangle
      {{-1, 0, TYELLOW}, {0, 0, TYELLOW}, {-1, -1, TYELLOW}, {0, -1, TYELLOW}},
      // blue bar
      {{0, 0, TBLUE}, {0, 1, TBLUE}, {0, 2, TBLUE}, {0, 3, TBLUE}},
      // red s
      {{-1,-1,TRED},{0,-1,TRED},{0,0,TRED},{1,0,TRED}},
      // green z
      {{-1,0,TGREEN},{0,0,TGREEN},{0,-1,TGREEN},{1,-1,TGREEN}},
      // orange l
      {{0,-1,TORANGE},{-1,-1,TORANGE},{-1,0,TORANGE},{-1,1,TORANGE}},
      // pink j
      {{0,1,TPINK},{0,0,TPINK},{0,-1,TPINK},{-1,-1,TPINK}},
      // brown t
      {{-1,0,TBROWN},{0,0,TBROWN},{1,0,TBROWN},{0,-1,TBROWN}},
  };
private:
  int variant;
};

#endif
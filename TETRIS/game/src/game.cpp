#include "game.h"
#include <random>

Game :: Game()
{
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock=GetRandomBlock();
    nextBlock=GetRandomBlock();
    gameOver = false;
    score =0;
    InitAudioDevice();
    music = LoadMusicStream("sounds/icardiiblmiyorumdiyor.mp3");
    PlayMusicStream(music);
  
}

Game::~Game()
{
    UnloadMusicStream(music);
    CloseAudioDevice();
}


Block Game::GetRandomBlock()
{
    if(blocks.empty())
    {
       blocks = GetAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin()+randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(),JBlock(),LBlock(),OBlock(),SBlock(),TBlock(),ZBlock()};
}

void Game ::Draw()
{
    grid.Draw();
    currentBlock.Draw(11,11);
    
    switch(nextBlock.id)
    {
        case 3:
        nextBlock.Draw(700,480);
        break;
        case 6:
        nextBlock.Draw(750,480);
        break;
        case 7:
        nextBlock.Draw(800,480);
        break;
        default:
        nextBlock.Draw(700,450);
        break;

    }
}

void Game::HandleInput()
{

    int keyPressed = GetKeyPressed();
    if(gameOver && keyPressed != 0)
    {
        gameOver = false;
        Reset();
    }
    switch(keyPressed)
    {
    case KEY_LEFT:
        MoveBlockLeft();
        break;
    case KEY_RIGHT:
        MoveBlockRight();
        break;
    case KEY_DOWN:
        MoveBlockDown();
        UpdateScore(0,1);
        break;
    case KEY_UP:
        RotateBlock();
        break;    
    }
}

void Game::MoveBlockLeft()
{
    if(!gameOver)
    { 
        currentBlock.Move(0 , -1);
        if(IsBlockOutside() || BlockFits() == false)
        {
             currentBlock.Move(0 ,1);
        }
    }
}

void Game::MoveBlockRight()
{
    if(!gameOver)
    {
        currentBlock.Move(0 , 1);
        if(IsBlockOutside() || BlockFits() == false)
        {
             currentBlock.Move(0 , -1);
        }
    }
}

void Game::MoveBlockDown()
{
    if(!gameOver)
        {
        currentBlock.Move(1 , 0);
        if(IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(-1 ,0);
            LockBlock();
        }
    }
}

bool Game::IsBlockOutside()
{
   std::vector<Position> tiles = currentBlock.GetCellPositions();
   for(Position item: tiles)
   {
        if(grid.IsCellOutside(item.row , item.column))
        {
            return true;
        }
        
   }
   return false;
}

void Game::RotateBlock()
{
    if(!gameOver)
    {
         currentBlock.Rotate();
         if(IsBlockOutside() || BlockFits() == false)
         {
            currentBlock.UndoRotation();
         }
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    if( BlockFits() == false)
    {
        gameOver = true;
    }
    nextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    UpdateScore(rowsCleared,0);
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        if(grid.IsCellEmpty(item.row , item.column) == false)
        {
            return false;
        }     
    }
    return true;
}

void Game::Reset()
{
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    score = 0;
}

void Game::UpdateScore(int linesCleared, int moveDownPoints)
{
    switch(linesCleared)
    {
    case 1:
        score += 100;
        break;
    case 2:
        score += 250;
        break;
    case 3:
        score += 500;
        break;
    default:
        break;    
    }
    score += moveDownPoints;
}

#include "game.h"
#include <random>

Game :: Game()
{
    //oyun alanı ve blokların baslangic degerleri
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock=GetRandomBlock();
    nextBlock=GetRandomBlock();
    gameOver = false;
    score =0;
    InitAudioDevice();
    music = LoadMusicStream("sounds/icardiiblmiyorumdiyor.mp3");
    PlayMusicStream(music);
    clearSound = LoadSound("");
  
}
//destructor yüklenen ses dosyalarını kapattım
Game::~Game()
{
    UnloadSound(clearSound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}

//rastgele blok döndür
Block Game::GetRandomBlock()
{
    if(blocks.empty())//bos olup olmadıgına bakmak icin bos ise yeniden alınır bloklar
    {
       blocks = GetAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin()+randomIndex);//yeni blok üretmek icin blocks vektorunden alır
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(),JBlock(),LBlock(),OBlock(),SBlock(),TBlock(),ZBlock()};
}
//oyunu ekrana çizdiren fonksiyon
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
//klavye girdileri
void Game::HandleInput()
{

    int keyPressed = GetKeyPressed();
    if(gameOver && keyPressed != 0)
    {//eger oyun biterse ya da kullanıcı tuşa basarsa sıfırlanıyor
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
//bloğu sabitleyip yeni blok getiren fonksiyon
void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    //eger yeni blok sığmazsa oyunu bitiriyor
    if( BlockFits() == false)
    {
        gameOver = true;
    }

    nextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    
    //dolu satırları temizle ve güncelle
    if(rowsCleared >0)
    {
        PlaySound(clearSound);
        UpdateScore(rowsCleared,0);
    }

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

//oyunu sıfırlayan

void Game::Reset()
{
    grid.Initialize();//oyun alanı
    blocks = GetAllBlocks();//blok havuzu
    currentBlock = GetRandomBlock();//mevcut blok yniden olusturuyor
    nextBlock = GetRandomBlock();//sonraki blok
    score = 0;
}

//skoru güncelleme
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

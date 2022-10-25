#include"DxLib.h"
#include"Map.h"
#include"game.h"
#include"Pad.h"
#include<cassert>
#include <iostream>
#include <fstream>

namespace
{
	//マップチップ1つのサイズ
	constexpr int kChipSize = 32;

	//チップの数
	constexpr int kBgNumX = Game::kScreenWidth / kChipSize;
	constexpr int kBgNumY = Game::kScreenHeight / kChipSize;

	//入出力ファイル名
	//const char* const kFileName = "map.bin";
	const char* const kFileName = "map.bin";
}

Map::Map():
	m_handle(-1),
	m_graphWidth(0),
	m_graphHeight(0),
	m_cursorNo(0),
	m_mapData(kBgNumX * kBgNumY,0),
	m_scrollX(0),
	m_scrollY(0)
{
}

Map::~Map()
{

}

void Map::load()
{
	m_handle = LoadGraph("data/mapchip.png");
	GetGraphSize(m_handle, &m_graphWidth, &m_graphHeight);
}

void Map::unload()
{
	DeleteGraph(m_handle);
}

void Map::update()
{
	int indexX = m_cursorNo % kBgNumX;
	int indexY = m_cursorNo / kBgNumX;

	if (Pad::isTrigger(PAD_INPUT_1))
	{
		//指定したマップチップの変更
		if (m_mapData[m_cursorNo] < chipNum() - 1)
		{
			m_mapData[m_cursorNo]++;
		}
	}
	if (Pad::isTrigger(PAD_INPUT_2))
	{
		//指定したマップチップの変更
		if (m_mapData[m_cursorNo] > 0)
		{
			m_mapData[m_cursorNo]--;
		}
	}
	if (Pad::isTrigger(PAD_INPUT_3))
	{
		//ファイルの出力
		//outputData();
		readData();
	}

#if false
	if (Pad::isTrigger(PAD_INPUT_UP))
	{
		if (indexY > 0)
		{
			m_cursorNo -= kBgNumX;
		}
	}
	if (Pad::isTrigger(PAD_INPUT_DOWN))
	{
		if (indexY < (kBgNumY -1))
		{
			m_cursorNo += kBgNumX;
		}
	}
	if (Pad::isTrigger(PAD_INPUT_LEFT))
	{
		if (indexX > 0)
		{
			m_cursorNo--;
		}
	}
	if (Pad::isTrigger(PAD_INPUT_RIGHT))
	{
		if (indexX < (kBgNumX - 1))
		{
			m_cursorNo++;
		}
	}
#else
	if (Pad::isPress(PAD_INPUT_UP))
	{
		//m_scrollY++;
		m_scrollY+= 10;
		if (m_scrollY > Game::kScreenHeight)
		{
			m_scrollY -= Game::kScreenHeight;
		}
	}
	if (Pad::isPress(PAD_INPUT_DOWN))
	{
		//m_scrollY--;
		m_scrollY-= 10;
		if (m_scrollY < -Game::kScreenHeight)
		{
			m_scrollY += Game::kScreenHeight;
		}
	}
	if (Pad::isPress(PAD_INPUT_LEFT))
	{
		//m_scrollX++;
		m_scrollX += 10;
		if (m_scrollX > Game::kScreenWidth)
		{
			m_scrollX -= Game::kScreenWidth;
		}
	}
	if (Pad::isPress(PAD_INPUT_RIGHT))
	{
		//m_scrollX--;
		m_scrollX -= 10;
		if (m_scrollX < -Game::kScreenWidth)
		{
			m_scrollX += Game::kScreenWidth;
		}
	}
#endif
}

void Map::draw()
{
	//m_scrollX > 0		右にずれている
	//m_scrollX < 0		左にずれている
	//m_scrollY < 0		下にずれている
	//m_scrollY > 0		上にずれている

	/*int indexX = 0;
	int indexY = 0;
	//
	indexX = -(m_scrollX/kChipSize);
	while (indexX < 0)	indexX += kBgNumX;
	//
	indexY = -(m_scrollY/kChipSize);
	while (indexY < 0)	indexY += kBgNumY;*/
	//
	//for (int x = 0; x < kBgNumX; x++)
	//{
	//	for (int y = 0; y < kBgNumY; y++)
	//	{
	//		const int chipNo = m_mapData[ y * kBgNumX + x];
	//		assert(chipNo >= 0);
	//		assert(chipNo < chipNum());
	//		int graphX = (chipNo % chipNumX()) * kChipSize;
	//		int graphY = (chipNo / chipNumX()) * kChipSize;
	//
	//		DrawRectGraph(x * kChipSize, y * kChipSize,
	//			graphX, graphY, kChipSize, kChipSize, 
	//			m_handle, true, false);
	//	}
	//}

#if false

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			int offsetX = x * Game::kScreenWidth + m_scrollX;
			int offsetY = y * Game::kScreenHeight + m_scrollY;
			drawMap(offsetX, offsetY);
		}
	}
#else
	int offsetX = m_scrollX;
	if (offsetX > 0)	offsetX -= Game::kScreenWidth;
	int offsetY = m_scrollY;
	if (offsetY > 0)	offsetY -= Game::kScreenHeight;
	
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			drawMap(offsetX + x * Game::kScreenWidth, offsetY + y * Game::kScreenHeight);
		}
	}



#endif
	drawCursor();
}

void Map::drawMap(int offsetX, int offsetY)
{

	for (int x = 0; x < kBgNumX; x++)
	{
		for (int y = 0; y < kBgNumY; y++)
		{
			const int chipNo = m_mapData[y * kBgNumX + x];
			assert(chipNo >= 0);
			assert(chipNo < chipNum());
			int graphX = (chipNo % chipNumX()) * kChipSize;
			int graphY = (chipNo / chipNumX()) * kChipSize;

			DrawRectGraph(x * kChipSize + offsetX, y * kChipSize + offsetY,
				graphX, graphY, kChipSize, kChipSize,
				m_handle, true, false);
		}
	}
}

void Map::drawCursor()
{
	int indexX = m_cursorNo % kBgNumX;
	int indexY = m_cursorNo / kBgNumX;

	int graphX = indexX  * kChipSize;
	int graphY = indexY  * kChipSize;

	DrawBox(graphX, graphY, graphX + kChipSize, graphY + kChipSize, GetColor(255, 0, 0), false);
}

int Map::chipNumX()
{
	return (m_graphWidth / kChipSize);
}

int Map::chipNumY()
{
	return (m_graphHeight / kChipSize);
}

int Map::chipNum()
{
	return(chipNumX() * chipNumY());
}

void Map::outputData()
{
	std::ofstream ofs(kFileName, std:: ios::binary);

	//ファイルの読み込みに失敗
	if (!ofs)
	{
		return;
	}

	ofs.write(reinterpret_cast<char*>(m_mapData.data()), sizeof(int) * kBgNumX * kBgNumY);
	ofs.close();
}

void Map::readData()
{
	std::ifstream ifs(kFileName, std::ios::binary);
	
	//ファイルの読み込みに失敗
	if (!ifs)
	{
		return;
	}

	ifs.read(reinterpret_cast<char*>(m_mapData.data()), sizeof(int) * kBgNumX * kBgNumY);
	ifs.close();
}

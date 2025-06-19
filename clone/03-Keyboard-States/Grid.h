#pragma once
#include "GameObject.h"
#include <vector>

#define CELL_WIDTH	96
#define CELL_HEIGHT	48

#define COLUMN_MAX	10
#define ROW_MAX			5
/*
	Grid is a set of cells next to each others
*/
class CGrid
{
	int map_width; // Thêm biến lưu kích thước map
	int map_height;
	int num_cols;   // Thêm biến lưu số cột, số hàng
	int num_rows;

	vector<LPGAMEOBJECT>** cells;
	vector<LPGAMEOBJECT> crossObject;
	static CGrid* __instance;
public:
	void Init(int map_width, int map_height);

	// Put the object in the suitable cell
	void Classify(LPGAMEOBJECT obj);

	void PutObjectIntoGrid(LPGAMEOBJECT obj, int row_index, int column_index);

	// Get objects in cells[x][y] based on camera position
	void GetObjectsInGrid(vector<LPGAMEOBJECT>& objects, float left, float top, float right, float bottom);

	void Clear();

	static CGrid* GetInstance();

	~CGrid();
};

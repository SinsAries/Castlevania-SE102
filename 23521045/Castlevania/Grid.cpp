#include "Grid.h"
#include <unordered_set>
#include "Brick.h"
#include "Utils.h"

#define CELL_WIDTH	96
#define CELL_HEIGHT	48

void CGrid::Init(int map_width, int map_height)
{
	this->map_width = map_width;
	this->map_height = map_height;

	this->num_cols = (int)ceil((float)map_width / CELL_WIDTH);
	this->num_rows = (int)ceil((float)map_height / CELL_HEIGHT);

	// Cấp phát động mảng 2 chiều
	cells = new vector<LPGAMEOBJECT>*[num_rows];
	for (int i = 0; i < num_rows; i++)
	{
		cells[i] = new vector<LPGAMEOBJECT>[num_cols];
	}
}


void CGrid::Classify(LPGAMEOBJECT obj)
{
	int beginCellColumn, beginCellRow, endCellColumn, endCellRow;
	float l, t, r, b;
	obj->GetBoundingBox(l, t, r, b);

	beginCellColumn = (int)(l / CELL_WIDTH);
	beginCellRow = (int)(t / CELL_HEIGHT);

	endCellColumn = (int)(r / CELL_WIDTH);
	endCellRow = (int)(b / CELL_HEIGHT);

	for (int i = beginCellRow; i <= endCellRow; i++)
	{
		for (int j = beginCellColumn; j <= endCellColumn; j++)
		{
			cells[i][j].push_back(obj);
		}
	}
}

void CGrid::PutObjectIntoGrid(LPGAMEOBJECT obj, int row_index, int column_index)
{
	if (obj == NULL)
		return;

	cells[row_index][column_index].push_back(obj);
}

void CGrid::GetObjectsInGrid(vector<LPGAMEOBJECT>& objects, float left, float top, float right, float bottom)
{
	// Dùng set để đảm bảo mỗi object chỉ được thêm một lần
	unordered_set<LPGAMEOBJECT> unique_objects;

	// ... (code tính firstCellColumn, firstCellRow, last... giữ nguyên) ...
	int firstCellColumn = (int)(left / CELL_WIDTH);
	int firstCellRow = (int)(top / CELL_HEIGHT);
	int lastCellColumn = (int)ceil(right / CELL_WIDTH);
	int lastCellRow = (int)ceil(bottom / CELL_HEIGHT);

	if (firstCellColumn < 0) firstCellColumn = 0;
	if (firstCellRow < 0) firstCellRow = 0;
	if (lastCellColumn >= this->num_cols) lastCellColumn = this->num_cols - 1;
	if (lastCellRow >= this->num_rows) lastCellRow = this->num_rows - 1;


	for (int row = firstCellRow; row <= lastCellRow; row++)
	{
		for (int column = firstCellColumn; column <= lastCellColumn; column++)
		{
			for (LPGAMEOBJECT obj : cells[row][column])
			{
				if (obj->IsVisible()) // Giả sử có hàm isVisible()
				{
					unique_objects.insert(obj);
				}
			}
		}
	}

	// Xóa danh sách cũ và điền các đối tượng duy nhất vào
	objects.clear();
	objects.assign(unique_objects.begin(), unique_objects.end());
}

void CGrid::Clear()
{
	for (int i = 0; i < num_rows; i++) // Dùng num_rows thay vì ROW_MAX
	{
		for (int j = 0; j < num_cols; j++) // Dùng num_cols thay vì COLUMN_MAX
		{
			cells[i][j].clear();
		}
	}
}

CGrid* CGrid::__instance = NULL;

CGrid* CGrid::GetInstance()
{
	if (__instance == NULL) __instance = new CGrid();
	return __instance;
}

CGrid::~CGrid()
{
	// Nếu mảng cells đã được cấp phát
	if (cells != nullptr)
	{
		// Vòng lặp để giải phóng từng hàng
		for (int i = 0; i < num_rows; i++)
		{
			delete[] cells[i]; // Giải phóng mảng các vector trong hàng i
		}
		// Sau đó giải phóng mảng các con trỏ hàng
		delete[] cells;
		cells = nullptr; // Gán về null để an toàn
	}
}
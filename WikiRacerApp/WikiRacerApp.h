#pragma once

#include "resource.h"
#include "WikiGraph.h"

#define IDC_FROM_EDIT 101
#define IDC_TO_EDIT 102
#define ID_SEARCH 103
#define ID_LOAD_DATA 109
#define DJI_SEARCH 111

class WikiRacerApp {
public:
    WikiRacerApp();
    ~WikiRacerApp(); 

    void Initialize(HWND hWnd);
    void PerformSearch(HWND hWnd);

private:
    WikiGraph graph;
};

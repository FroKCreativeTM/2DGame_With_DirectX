#pragma once

#define SAFE_RELEASE(x) { if(x) { (x)->Release(); (x) = nullptr; } }
#define SAFE_DELETE(x) { if(x) { delete (x); (x) = nullptr; } }
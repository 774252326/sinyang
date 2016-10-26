#pragma once

#include <afxcmn.h>
#include <vector>

BOOL ReadFileCustom(CObject * co, size_t n, CString fp);
BOOL WriteFileCustom(CObject * co, size_t n, CString fp);
void LoadFileList(const CString &m_filePath, std::vector<CString> &filelist);
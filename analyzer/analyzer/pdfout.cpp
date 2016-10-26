#include "stdafx.h"

#include <iostream>
#include "resource.h"
#include "pdfout.h"

//#import "p/pdflib.dll" no_namespace

//using namespace std;
//using namespace pdflib;


int pdfout(pdflib::PDFlib &p, const std::vector<DataOut> &dol){

	//const std::wstring searchpath = L"../data";

	int row, col, font, image, tf=-1, tbl=-1;
	int rowmax=dol.size()+2, colmax=7;
	//PDFlib p;
	double llx=50, lly=50, urx=550, ury=800;

	CString str;

	str.LoadStringW(IDS_OUTPUT_WND);

	const std::wstring headertext = (LPCWSTR)str;
	//L"Table header (centered across all columns)";
	std::wstring result;
	std::wostringstream optlist;

	/* -------------------- Add table cells -------------------- */

	/* ---------- Row 1: table header (spans all columns) */
	row = 1; col = 1;
	font = p.load_font(L"Times-Bold", L"winansi", L"");
	if (font == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return(2);
	}

	optlist.str(L"");
	optlist << L"fittextline={position=center font=" << font
		<< L" fontsize=14} colspan=" << colmax;

	tbl = p.add_table_cell(tbl, col, row, headertext, optlist.str());

	row++;



	for(col = 1;col<=7;col++)
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;

		str.LoadStringW(IDS_STRING111+col-1);

		num << (LPCWSTR)str;

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
	}


	for (row++; row <= rowmax; row++) {

		//for (col = 1; col <= colmax; col++) 

		col = 1;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			num << row-3;

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


		col = 2;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			num << /*(LPWSTR)_bstr_t*/(LPCWSTR)(dol[row-3].stepName);

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());

			//tbl = p.add_table_cell(tbl, col, row, , optlist.str());
		}


		col = 3;
		if(dol[row-3].addVol!=0)
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			num << dol[row-3].addVol;

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


		col = 4;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			num << dol[row-3].totalVol;

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


		col = 5;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			num << dol[row-3].Q;

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


		col = 6;
		if(dol[row-3].nQ!=0 && dol[row-3].Use)
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			num << dol[row-3].nQ;

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}

		col = 7;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			num << (dol[row-3].Use? L"Yes":L"No");

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


	}

	/* ---------- Place the table on one or more pages ---------- */

	/*
	* Loop until all of the table is placed; create new pages
	* as long as more table instances need to be placed.
	*/
	do {
		p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

		/* Shade every other row; draw lines for all table cells.
		* Add "showcells showborder" to visualize cell borders 
		*/

		/* Place the table instance */
		result = p.fit_table(tbl, llx, lly, urx, ury, 
			L"header=1 fill={{area=rowodd fillcolor={gray 0.9}}} " 
			L"stroke={{line=other}}");
		if (result == L"_error") {
			std::wcerr << L"Couldn't place table: " << p.get_errmsg() << std::endl;
			return 2;
		}

		p.end_page_ext(L"");

	} while (result == L"_boxfull");

	/* Check the result; "_stop" means all is ok. */
	if (result != L"_stop") {
		if (result == L"_error") {
			std::wcerr << L"Error when placing table: " << p.get_errmsg()
				<< std::endl;
			return 2;
		}
		else {
			/* Any other return value is a user exit caused by
			* the "return" option; this requires dedicated code to
			* deal with.
			*/
			std::wcerr << L"User return found in Textflow" << std::endl;
			return 2;
		}
	}
	/* This will also delete Textflow handles used in the table */
	p.delete_table(tbl, L"");

	return 0;
}



int pdfout1(pdflib::PDFlib &p, const SAPara &para){

	//const std::wstring searchpath = L"../data";

	int row, col, font, image, tf=-1, tbl=-1;
	int rowmax=para.saplist.size()+2, colmax=7;
	//PDFlib p;
	double llx=50, lly=50, urx=550, ury=800;

	CString str;

	str.LoadStringW(IDS_STRING_ADDITION_SOLUTION_PARA);

	const std::wstring headertext = (LPCWSTR)str;
	//L"Table header (centered across all columns)";
	std::wstring result;
	std::wostringstream optlist;

	/* -------------------- Add table cells -------------------- */

	/* ---------- Row 1: table header (spans all columns) */
	row = 1; col = 1;
	font = p.load_font(L"Times-Bold", L"winansi", L"");
	if (font == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return(2);
	}

	optlist.str(L"");
	optlist << L"fittextline={position=center font=" << font
		<< L" fontsize=14} colspan=" << colmax;

	tbl = p.add_table_cell(tbl, col, row, headertext, optlist.str());

	row++;



	for(col = 1;col<=7;col++)
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;

		str.LoadStringW(IDS_STRING520+col-1);

		num << (LPCWSTR)str;

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
	}


	for (row++; row <= rowmax; row++) {

		//for (col = 1; col <= colmax; col++) 

		col = 1;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			num << row-3;

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


		col = 2;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			str.LoadStringW(IDS_STRING_VOL_ONCE+para.saplist[row-3].addType);
			num <<(LPCWSTR)(str);

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());

			//tbl = p.add_table_cell(tbl, col, row, , optlist.str());
		}


		col = 3;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;
			str.Format(L"%g",para.saplist[row-3].Sconc);
			num << (LPCWSTR)(str);

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


		col = 4;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;
			str.Format(L"%g",para.saplist[row-3].Aconc);
			num << (LPCWSTR)(str);

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


		col = 5;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;
			str.Format(L"%g",para.saplist[row-3].Lconc);
			num << (LPCWSTR)(str);

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


		col = 6;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;
			str.Format(L"%g",para.saplist[row-3].volconc);
			num << (LPCWSTR)(str);

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}

		col = 7;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;
			str.Format(L"%g",para.saplist[row-3].endRatio);
			num << (LPCWSTR)(str);

			optlist.str(L"");
			optlist << L"colwidth=20% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


	}

	/* ---------- Place the table on one or more pages ---------- */

	/*
	* Loop until all of the table is placed; create new pages
	* as long as more table instances need to be placed.
	*/
	do {
		p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

		/* Shade every other row; draw lines for all table cells.
		* Add "showcells showborder" to visualize cell borders 
		*/

		/* Place the table instance */
		result = p.fit_table(tbl, llx, lly, urx, ury, 
			L"header=1 fill={{area=rowodd fillcolor={gray 0.9}}} " 
			L"stroke={{line=other}}");
		if (result == L"_error") {
			std::wcerr << L"Couldn't place table: " << p.get_errmsg() << std::endl;
			return 2;
		}

		p.end_page_ext(L"");

	} while (result == L"_boxfull");

	/* Check the result; "_stop" means all is ok. */
	if (result != L"_stop") {
		if (result == L"_error") {
			std::wcerr << L"Error when placing table: " << p.get_errmsg()
				<< std::endl;
			return 2;
		}
		else {
			/* Any other return value is a user exit caused by
			* the "return" option; this requires dedicated code to
			* deal with.
			*/
			std::wcerr << L"User return found in Textflow" << std::endl;
			return 2;
		}
	}
	/* This will also delete Textflow handles used in the table */
	p.delete_table(tbl, L"");

	return 0;
}


int pdfout2(pdflib::PDFlib &p, const CVPara &para){

	//const std::wstring searchpath = L"../data";

	int row, col, font, image, tf=-1, tbl=-1;
	int rowmax=3, colmax=6;
	//PDFlib p;
	double llx=50, lly=50, urx=550, ury=800;

	CString str;

	str.LoadStringW(IDS_STRING_CV_PARA);

	const std::wstring headertext = (LPCWSTR)str;
	//L"Table header (centered across all columns)";
	std::wstring result;
	std::wostringstream optlist;

	/* -------------------- Add table cells -------------------- */

	/* ---------- Row 1: table header (spans all columns) */
	row = 1; col = 1;
	font = p.load_font(L"Times-Bold", L"winansi", L"");
	if (font == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return(2);
	}

	optlist.str(L"");
	optlist << L"fittextline={position=center font=" << font
		<< L" fontsize=14} colspan=" << colmax;

	tbl = p.add_table_cell(tbl, col, row, headertext, optlist.str());

	row++;







	col = 1;
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;

		str.LoadStringW(IDS_STRING_LOW_E_LIMIT);
		num <<(LPCWSTR)(str);

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
	}


	col = 2;
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;

		str.LoadStringW(IDS_STRING_HIGH_E_LIMIT);
		num <<(LPCWSTR)(str);

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());

		//tbl = p.add_table_cell(tbl, col, row, , optlist.str());
	}


	col = 3;
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;
		str.LoadStringW(IDS_STRING_SCAN_RATE);
		num <<(LPCWSTR)(str);

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
	}


	col = 4;
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;
		str.LoadStringW(para.combochoice==0?IDS_STRING_NO_OF_CYCLES:IDS_STRING_VARIATION_TOLERANCE);
		num <<(LPCWSTR)(str);

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
	}


	col = 5;
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;
		str.LoadStringW(IDS_STRING_ROTATION_RATE);
		num <<(LPCWSTR)(str);

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
	}


	col = 6;
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;
		str.LoadStringW(IDS_STRING_END_INTEGRATION_E);
		num <<(LPCWSTR)(str);

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
	}




	row++;
	col = 1;
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;


		num <<para.lowelimit;

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
	}


	col = 2;
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;

		//str.LoadStringW(IDS_STRING_HIGH_E_LIMIT);
		num <<para.highelimit;

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());

		//tbl = p.add_table_cell(tbl, col, row, , optlist.str());
	}


	col = 3;
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;
		//str.LoadStringW(IDS_STRING_SCAN_RATE);
		num <<para.scanrate;

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
	}


	col = 4;
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;
		if(para.combochoice==0)
			num<<para.noofcycles;
		else
			num<<para.variationtolerance;

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
	}


	col = 5;
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;
		//str.LoadStringW(IDS_STRING_ROTATION_RATE);
		num <<para.rotationrate;

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
	}


	col = 6;
	{
		std::wostringstream num;

		//num << L"Col " << col << L"/Row " << row;
		//str.LoadStringW(IDS_STRING_END_INTEGRATION_E);
		num <<para.endintegratione;

		optlist.str(L"");
		optlist << L"colwidth=20% fittextline={font=" << font
			<<  L" fontsize=10}";

		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
	}


	/* ---------- Place the table on one or more pages ---------- */

	/*
	* Loop until all of the table is placed; create new pages
	* as long as more table instances need to be placed.
	*/
	do {
		p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

		/* Shade every other row; draw lines for all table cells.
		* Add "showcells showborder" to visualize cell borders 
		*/

		/* Place the table instance */
		result = p.fit_table(tbl, llx, lly, urx, ury, 
			L"header=1 fill={{area=rowodd fillcolor={gray 0.9}}} " 
			L"stroke={{line=other}}");
		if (result == L"_error") {
			std::wcerr << L"Couldn't place table: " << p.get_errmsg() << std::endl;
			return 2;
		}

		p.end_page_ext(L"");

	} while (result == L"_boxfull");

	/* Check the result; "_stop" means all is ok. */
	if (result != L"_stop") {
		if (result == L"_error") {
			std::wcerr << L"Error when placing table: " << p.get_errmsg()
				<< std::endl;
			return 2;
		}
		else {
			/* Any other return value is a user exit caused by
			* the "return" option; this requires dedicated code to
			* deal with.
			*/
			std::wcerr << L"User return found in Textflow" << std::endl;
			return 2;
		}
	}
	/* This will also delete Textflow handles used in the table */
	p.delete_table(tbl, L"");

	return 0;
}



int pdfout3(pdflib::PDFlib &p, const ANPara &para, const CString &resultStr){

	int font;
	font = p.load_font(L"Helvetica", L"winansi", L"");
	if (font == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}

	/* Start page 1 */
	p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");
	p.setfont(font, 12);

	CString str;

	int hi=800;

	str.LoadStringW(IDS_STRING_AT1+para.analysistype);
	p.fit_textline((LPCWSTR)str, 20, hi, L"");

	hi-=12;
	str.LoadStringW(IDS_STRING_EVALUATION_RATIO);
	{
		CString str1;			
		str1.Format(L"=%g",para.evaluationratio);
		str=str+str1;
	}
	p.fit_textline((LPCWSTR)str, 20, hi, L"");
	hi-=12;

	switch(para.analysistype){
	case 2:
	case 6:
	case 8:
	case 10:
		str.LoadStringW(IDS_STRING_CALIBRATION_CURVE_FILE);
		p.fit_textline((LPCWSTR)str, 20, hi, L"");
		hi-=12;
		p.fit_textline((LPCWSTR)para.calibrationfilepath, 20, hi, L"");
		hi-=12;
		break;
	case 4:
		str.LoadStringW(IDS_STRING_INTERCEPT_VALUE);
		{
			CString str1;			
			str1.Format(L"=%g",para.interceptvalue);
			str=str+str1;
		}
		p.fit_textline((LPCWSTR)str, 20, hi, L"");
		hi-=12;
		break;
	default:
		break;
	}
	p.fit_textline((LPCWSTR)resultStr, 20, hi, L"");


	p.end_page_ext(L"");

	return 0;
}



int imgout(pdflib::PDFlib &p, const std::wstring imagefile)
{
	int font, image;
	std::wostringstream optlist;

	image = p.load_image(L"auto", imagefile, L"");
	if (image == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}
	p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

	/* Position the image in its original size with its lower left corner
	* at the reference point (20, 380)
	*/

	optlist.str(L"");
	optlist << L"boxsize={500 750}"
		<< L" fitmethod=auto"
		<< L" position={left top}";

	p.fit_image(image, 50, 50, optlist.str());

	p.end_page_ext(L"");

	return 0;
}


int pdfd(CString outfile, CanalyzerDoc *padoc)
{
	const std::wstring searchpath = L"../data";

	pdflib::PDFlib p;

	p.set_parameter(L"errorpolicy", L"return");

	p.set_parameter(L"SearchPath", searchpath);

	if (p.begin_document((LPCWSTR)outfile, L"") == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}
	p.set_info(L"Creator", L"PDFlib starter sample");
	p.set_info(L"Title", L"starter_table");

	int a;

	a=pdfout3(p,padoc->p1,padoc->resultStr);
	a=pdfout(p,padoc->dol);
	a=pdfout1(p,padoc->p3);
	a=pdfout2(p,padoc->p2);

	CSize szimg(800,600);
	//CSize szimg(1200,900);
	//CSize szimg(600,450);

	CString fptmp=L"temp.bmp";

	for(size_t i=0;i<padoc->lp.size();i++){
		padoc->SaveImage(padoc->lp[i],szimg,fptmp);
		imgout(p,(LPCWSTR)fptmp);
	}

	for(size_t i=0;i<padoc->rp.size();i++){
		padoc->SaveImage(padoc->rp[i],szimg,fptmp);
		imgout(p,(LPCWSTR)fptmp);
	}

	CFile::Remove(fptmp);

	p.end_document(L"");

	return 0;
}
#include "stdafx.h"


#include "resource.h"
#include "pdfout.h"

#include "func.h"

//#import "p/pdflib.dll" no_namespace

//using namespace std;
//using namespace pdflib;



//const std::wstring logofile = L"res\\sinyang.bmp";
//const std::wstring logofile = L"res\\index_02_01_01.jpg";
const std::wstring logofile = L"templogo.bmp";








int AddPageNumber(const std::wstring fin, const std::wstring fout)
{
	/* This is where the data files are. Adjust as necessary. */
	const std::wstring searchpath = L"../data";

	pdflib::PDFlib p;

	//  This means we must check return values of load_font() etc.
	p.set_parameter(L"errorpolicy", L"return");

	p.set_parameter(L"SearchPath", searchpath);

	if (p.begin_document(fout, L"") == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}
	p.set_info(L"Creator", L"PDFlib starter sample");
	p.set_info(L"Title", L"starter_pdfmerge");


	int indoc, endpage, pageno, page; 

	/* Open the input PDF */

	indoc = p.open_pdi_document(fin, L"");
	if (indoc == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}

	endpage = static_cast<int>(p.pcos_get_number(indoc, L"length:pages"));


	/* Loop over all pages of the input document */
	for (pageno = 1; pageno <= endpage; pageno++)
	{
		page = p.open_pdi_page(indoc, pageno, L"");
		if (page == -1) {
			std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
			continue;
		}
		/* Dummy page size; will be adjusted later */
		p.begin_page_ext(10, 10, L"");

		/* Place the imported page on the output page, and
		* adjust the page size
		*/
		p.fit_pdi_page(page, 0, 0, L"adjustpage");
		p.close_pdi_page(page);


		int font = p.load_font(L"Times-Bold", L"winansi", L"");

		p.setfont(font, 10);
		std::wostringstream strt;
		strt<<L"page "<<pageno<<L" of "<<endpage;

		p.fit_textline(strt.str(), 20, 20, L"");

		p.end_page_ext(L"");
	}
	p.close_pdi_document(indoc);


	p.end_document(L"");

	return 0;
}






//
//
//
//
//int pdfout(pdflib::PDFlib &p, const std::vector<DataOut> &dol){
//
//	//const std::wstring searchpath = L"../data";
//
//	int row, col, font, image, tf=-1, tbl=-1;
//	int rowmax=dol.size()+2, colmax=7;
//	//PDFlib p;
//	double llx=50, lly=50, urx=550, ury=800;
//
//	CString str;
//
//	str.LoadStringW(IDS_OUTPUT_WND);
//
//	const std::wstring headertext = (LPCWSTR)str;
//	//L"Table header (centered across all columns)";
//	std::wstring result;
//	std::wostringstream optlist;
//
//	/* -------------------- Add table cells -------------------- */
//
//	/* ---------- Row 1: table header (spans all columns) */
//	row = 1; col = 1;
//	font = p.load_font(L"Times-Bold", L"winansi", L"");
//	if (font == -1) {
//		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
//		return(2);
//	}
//
//	optlist.str(L"");
//	optlist << L"fittextline={position=center font=" << font
//		<< L" fontsize=14} colspan=" << colmax;
//
//	tbl = p.add_table_cell(tbl, col, row, headertext, optlist.str());
//
//	row++;
//
//
//
//	for(col = 1;col<=7;col++)
//	{
//		std::wostringstream num;
//
//		//num << L"Col " << col << L"/Row " << row;
//
//		str.LoadStringW(IDS_STRING111+col-1);
//
//		num << (LPCWSTR)str;
//
//		optlist.str(L"");
//		optlist << L"colwidth=20% fittextline={font=" << font
//			<<  L" fontsize=10}";
//
//		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
//	}
//
//
//	for (row++; row <= rowmax; row++) {
//
//		//for (col = 1; col <= colmax; col++) 
//
//		col = 1;
//		{
//			std::wostringstream num;
//
//			//num << L"Col " << col << L"/Row " << row;
//
//			num << row-3;
//
//			optlist.str(L"");
//			optlist << L"colwidth=20% fittextline={font=" << font
//				<<  L" fontsize=10}";
//
//			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
//		}
//
//
//		col = 2;
//		{
//			std::wostringstream num;
//
//			//num << L"Col " << col << L"/Row " << row;
//
//			num << /*(LPWSTR)_bstr_t*/(LPCWSTR)(dol[row-3].stepName);
//
//			optlist.str(L"");
//			optlist << L"colwidth=20% fittextline={font=" << font
//				<<  L" fontsize=10}";
//
//			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
//
//			//tbl = p.add_table_cell(tbl, col, row, , optlist.str());
//		}
//
//
//		col = 3;
//		if(dol[row-3].addVol!=0)
//		{
//			std::wostringstream num;
//
//			//num << L"Col " << col << L"/Row " << row;
//
//			num << dol[row-3].addVol;
//
//			optlist.str(L"");
//			optlist << L"colwidth=20% fittextline={font=" << font
//				<<  L" fontsize=10}";
//
//			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
//		}
//
//
//		col = 4;
//		{
//			std::wostringstream num;
//
//			//num << L"Col " << col << L"/Row " << row;
//
//			num << dol[row-3].totalVol;
//
//			optlist.str(L"");
//			optlist << L"colwidth=20% fittextline={font=" << font
//				<<  L" fontsize=10}";
//
//			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
//		}
//
//
//		col = 5;
//		{
//			std::wostringstream num;
//
//			//num << L"Col " << col << L"/Row " << row;
//
//			num << dol[row-3].Q;
//
//			optlist.str(L"");
//			optlist << L"colwidth=20% fittextline={font=" << font
//				<<  L" fontsize=10}";
//
//			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
//		}
//
//
//		col = 6;
//		if(dol[row-3].nQ!=0 && dol[row-3].Use)
//		{
//			std::wostringstream num;
//
//			//num << L"Col " << col << L"/Row " << row;
//
//			num << dol[row-3].nQ;
//
//			optlist.str(L"");
//			optlist << L"colwidth=20% fittextline={font=" << font
//				<<  L" fontsize=10}";
//
//			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
//		}
//
//		col = 7;
//		{
//			std::wostringstream num;
//
//			//num << L"Col " << col << L"/Row " << row;
//
//			num << (dol[row-3].Use? L"Yes":L"No");
//
//			optlist.str(L"");
//			optlist << L"colwidth=20% fittextline={font=" << font
//				<<  L" fontsize=10}";
//
//			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
//		}
//
//
//	}
//
//	/* ---------- Place the table on one or more pages ---------- */
//
//	/*
//	* Loop until all of the table is placed; create new pages
//	* as long as more table instances need to be placed.
//	*/
//	do {
//		p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");
//
//		/* Shade every other row; draw lines for all table cells.
//		* Add "showcells showborder" to visualize cell borders 
//		*/
//
//		/* Place the table instance */
//		result = p.fit_table(tbl, llx, lly, urx, ury, 
//			L"header=1 fill={{area=rowodd fillcolor={gray 0.9}}} " 
//			L"stroke={{line=other}}");
//		if (result == L"_error") {
//			std::wcerr << L"Couldn't place table: " << p.get_errmsg() << std::endl;
//			return 2;
//		}
//
//		p.end_page_ext(L"");
//
//	} while (result == L"_boxfull");
//
//	/* Check the result; "_stop" means all is ok. */
//	if (result != L"_stop") {
//		if (result == L"_error") {
//			std::wcerr << L"Error when placing table: " << p.get_errmsg()
//				<< std::endl;
//			return 2;
//		}
//		else {
//			/* Any other return value is a user exit caused by
//			* the "return" option; this requires dedicated code to
//			* deal with.
//			*/
//			std::wcerr << L"User return found in Textflow" << std::endl;
//			return 2;
//		}
//	}
//	/* This will also delete Textflow handles used in the table */
//	p.delete_table(tbl, L"");
//
//	return 0;
//}
//
//


int pdfout(pdflib::PDFlib &p, const std::vector<DataOutA> &dol){

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
	row = 1; col = 1;
	/* ---------- Row 1: table header (spans all columns) */





	//const std::wstring imagefile = L"C:\\Users\\r8anw2x\\Documents\\Visual Studio 2010\\project0\\analyzer\\analyzer\\res\\sinyang.bmp";

	image = p.load_image(L"auto", logofile, L"");
	if (image == -1) {
		std::wcerr << L"Couldn't load image: " << p.get_errmsg() << std::endl;
		return 2;
	}

	optlist.str(L"");
	optlist << L"image=" << image << L" fitimage={fitmethod=auto position={right center}}";
	optlist << L" rowheight=30";
	optlist << L" colspan=" << colmax;

	tbl = p.add_table_cell(tbl, col, row, L"", optlist.str());
	if (tbl == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}

	row++;




	font = p.load_font(L"Helvetica", L"winansi", L"");
	if (font == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return(2);
	}

	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font
		<< L" fontsize=10} colspan=" << colmax;

	tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());
	row++;

	tbl = p.add_table_cell(tbl, col, row, headertext, optlist.str());

	row++;

	int rowstart=row;

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


	for (row++; row <= rowstart+dol.size(); row++) {

		//for (col = 1; col <= colmax; col++) 

		int di=row-rowstart-1;

		DataOutA doa=dol[di];


		col = 1;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			num << di;

			optlist.str(L"");
			optlist << L"colwidth=15% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


		col = 2;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			num << /*(LPWSTR)_bstr_t*/(LPCWSTR)(doa.GetStepName(di));

			optlist.str(L"");
			optlist << L"colwidth=15% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());

			//tbl = p.add_table_cell(tbl, col, row, , optlist.str());
		}


		col = 3;
		if(dol[di].addVolume!=0)
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			num << dol[di].addVolume;

			optlist.str(L"");
			optlist << L"colwidth=15% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


		col = 4;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			num << doa.TotalVolume();
			optlist.str(L"");
			optlist << L"colwidth=15% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


		col = 5;
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			if( dol[di].UseIndex>=0 && dol[di].UseIndex<dol[di].Ar.size() )
				num << dol[di].Ar[dol[di].UseIndex];
			else
				num << dol[di].Ar.back();

			optlist.str(L"");
			optlist << L"colwidth=15% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}


		col = 6;
		//if(dol[row-3].nQ!=0 && dol[row-3].Use)
		//if(dol[di].Ar.back()!=0)
		{
			std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;

			//num << dol[di].Ar.back()/dol[di].Ar0;

			if( dol[di].UseIndex>=0 && dol[di].UseIndex<dol[di].Ar.size() )
				num << dol[di].Ar[dol[di].UseIndex]/dol[di].Ar0;
			else
				num << L" ";

			optlist.str(L"");
			optlist << L"colwidth=15% fittextline={font=" << font
				<<  L" fontsize=10}";

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
		}

		col = 7;
		{
			//std::wostringstream num;

			//num << L"Col " << col << L"/Row " << row;
			CString st;
			//num << L"Yes";
			if( dol[di].UseIndex>=0 && dol[di].UseIndex<dol[di].Ar.size() )
				st.LoadStringW(IDS_STRING_YES);
			else
				st.LoadStringW(IDS_STRING_NO);



			optlist.str(L"");
			optlist << L"colwidth=15% fittextline={font=" << font
				<<  L" fontsize=10}";

			//tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
			tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)st, optlist.str());
		}


	}


	int rowend=row;




	optlist.str(L"");
	optlist <<L"fill={";
	for(int i=rowstart;i<rowend;i+=2){
		optlist << L"{area=row"
			<< i
			<< L" fillcolor={gray 0.9}} ";
	}
	optlist << L"} "; 
	//optlist << L"stroke={{line=other}} ";



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
		result = p.fit_table(tbl, llx, lly, urx, ury, optlist.str());
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
			//L"stroke={{line=other}}"
			);
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



int pdfout4(pdflib::PDFlib &p, const ANPara &para, const std::vector<DataOutA> &dol){


	int fsize=12;


	int font;
	font = p.load_font(L"Helvetica", L"winansi", L"");
	if (font == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}

	/* Start page 1 */
	p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");
	p.setfont(font, fsize);

	CString str;

	int hi=800;
	int hi2=50;
	int wi=50;
	int wi2=175;
	int wi3=300;
	int wi4=425;
	int wi5=550;

	str.LoadStringW(IDS_STRING_REPORT);
	p.fit_textline((LPCWSTR)str, wi3-30, hi, L"");
	hi-=fsize;


	str.LoadStringW(IDS_STRING_GENERAL_INFO);
	p.fit_textline((LPCWSTR)str, wi, hi, L"");
	hi-=fsize;

	str.LoadStringW(IDS_STRING_SOFTWARE);
	p.fit_textline((LPCWSTR)str, wi, hi, L"");

	str.LoadStringW(AFX_IDS_APP_TITLE);
	p.fit_textline((LPCWSTR)str, wi2, hi, L"");

	str.LoadStringW(IDS_STRING_TIME);
	p.fit_textline((LPCWSTR)str, wi3, hi, L"");

	str=TimeString(true);
	p.fit_textline((LPCWSTR)str, wi4, hi, L"");

	hi-=fsize;
	hi-=fsize;

	str.LoadStringW(IDS_STRING_ANALYSIS_PARA);
	p.fit_textline((LPCWSTR)str, wi, hi, L"");
	hi-=fsize;

	str.LoadStringW(IDS_STRING_ANALYSIS_TYPE);
	p.fit_textline((LPCWSTR)str, wi, hi, L"");

	str.LoadStringW(IDS_STRING_AT1+para.analysistype);
	p.fit_textline((LPCWSTR)str, wi2, hi, L"");
	hi-=fsize;

	str.LoadStringW(IDS_STRING_EVALUATION_RATIO);
	p.fit_textline((LPCWSTR)str, wi, hi, L"");

	str.Format(L"%g",para.evaluationratio);
	p.fit_textline((LPCWSTR)str, wi2, hi, L"");
	hi-=fsize;

	switch(para.analysistype){
	case 2:
		{
			switch(para.calibrationfactortype){
			case 0:
				{
					str.LoadStringW(IDS_STRING_CALIBRATION_FACTOR);
					p.fit_textline((LPCWSTR)str, wi, hi, L"");

					str.Format(L"%g",para.calibrationfactor);
					p.fit_textline((LPCWSTR)str, wi, hi, L"");
					hi-=fsize;

				}
				break;
			case 1:
				{
					str.LoadStringW(IDS_STRING_CALIBRATION_CURVE_FILE);
					p.fit_textline((LPCWSTR)str, wi, hi, L"");
					//hi-=fsize;
					p.fit_textline((LPCWSTR)para.calibrationfilepath, wi2, hi, L"");
					hi-=fsize;
				}
				break;
			default:
				break;
			}
			break;
		}

	case 6:
	case 8:
	case 10:
		str.LoadStringW(IDS_STRING_CALIBRATION_CURVE_FILE);
		p.fit_textline((LPCWSTR)str, wi, hi, L"");
		//hi-=fsize;
		p.fit_textline((LPCWSTR)para.calibrationfilepath, wi2, hi, L"");
		hi-=fsize;
		break;
	case 4:
		str.LoadStringW(IDS_STRING_INTERCEPT_VALUE);
		p.fit_textline((LPCWSTR)str, wi, hi, L"");

		str.Format(L"%g",para.interceptvalue);
		p.fit_textline((LPCWSTR)str, wi2, hi, L"");
		hi-=fsize;

		break;
	default:
		break;
	}
	//p.fit_textline((LPCWSTR)resultStr, 20, hi, L"");


	hi-=fsize;

	str.LoadStringW(IDS_STRING_RESULT);
	p.fit_textline((LPCWSTR)str, wi, hi, L"");
	hi-=fsize;

	std::vector<CString> res;
	bool flg=Compute(dol, para, res);

	for(size_t i=0;i<res.size();i+=2){
		p.fit_textline((LPCWSTR)(res[i]), wi, hi, L"");
		p.fit_textline((LPCWSTR)(res[i+1]), wi2, hi, L"");
		hi-=fsize;
	}



	p.end_page_ext(L"");

	return 0;
}




int pdfout5(pdflib::PDFlib &p, const ANPara &para, const std::vector<DataOutA> &dol, const CVPara &p2){


	int fsize=16;


	int font;
	font = p.load_font(L"Helvetica", L"winansi", L"");
	if (font == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}

	/* Start page 1 */
	p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");
	p.setfont(font, fsize);

	CString str;

	int hi=800;
	int hi2=50;
	int wi=50;
	int wi1=60;

	int wi2=175;
	int wi3=300;
	int wi4=425;
	int wi5=550;

	str.LoadStringW(IDS_STRING_REPORT);
	p.fit_textline((LPCWSTR)str, wi3-30, hi, L"");
	hi-=fsize;
	hi-=fsize;


	fsize=10;
	p.setfont(font, fsize);


	str.LoadStringW(IDS_STRING_GENERAL_INFO);
	p.fit_textline((LPCWSTR)str, wi, hi, L"");
	hi-=fsize;
	hi-=fsize;

	str.LoadStringW(IDS_STRING_SOFTWARE);
	p.fit_textline((LPCWSTR)str, wi1, hi, L"");

	str.LoadStringW(AFX_IDS_APP_TITLE);
	p.fit_textline((LPCWSTR)str, wi2, hi, L"");

	str.LoadStringW(IDS_STRING_TIME);
	p.fit_textline((LPCWSTR)str, wi3, hi, L"");

	str=TimeString(true);
	p.fit_textline((LPCWSTR)str, wi4, hi, L"");

	hi-=fsize;
	hi-=fsize;

	//p.setfont(font, 8);

	str.LoadStringW(IDS_STRING_ANALYSIS_PARA);
	p.fit_textline((LPCWSTR)str, wi, hi, L"");
	hi-=fsize;
	hi-=fsize;

	str.LoadStringW(IDS_STRING_ANALYSIS_TYPE);
	p.fit_textline((LPCWSTR)str, wi1, hi, L"");

	str.LoadStringW(IDS_STRING_AT1+para.analysistype);
	p.fit_textline((LPCWSTR)str, wi2, hi, L"");
	hi-=fsize;

	str.LoadStringW(IDS_STRING_EVALUATION_RATIO);
	p.fit_textline((LPCWSTR)str, wi1, hi, L"");

	str.Format(L"%g",para.evaluationratio);
	p.fit_textline((LPCWSTR)str, wi2, hi, L"");
	hi-=fsize;

	switch(para.analysistype){
	case 2:
		{
			switch(para.calibrationfactortype){
			case 0:
				{
					str.LoadStringW(IDS_STRING_CALIBRATION_FACTOR);
					p.fit_textline((LPCWSTR)str, wi1, hi, L"");

					str.Format(L"%g",para.calibrationfactor);
					p.fit_textline((LPCWSTR)str, wi2, hi, L"");
					hi-=fsize;

				}
				break;
			case 1:
				{
					str.LoadStringW(IDS_STRING_CALIBRATION_CURVE_FILE);
					p.fit_textline((LPCWSTR)str, wi1, hi, L"");
					//hi-=fsize;
					p.fit_textline((LPCWSTR)para.calibrationfilepath, wi2, hi, L"");
					hi-=fsize;
				}
				break;
			default:
				break;
			}
			break;
		}

	case 6:
	case 8:
	case 10:
		str.LoadStringW(IDS_STRING_CALIBRATION_CURVE_FILE);
		p.fit_textline((LPCWSTR)str, wi1, hi, L"");
		//hi-=fsize;
		p.fit_textline((LPCWSTR)para.calibrationfilepath, wi2, hi, L"");
		hi-=fsize;
		break;
	case 4:
		str.LoadStringW(IDS_STRING_INTERCEPT_VALUE);
		p.fit_textline((LPCWSTR)str, wi1, hi, L"");

		str.Format(L"%g",para.interceptvalue);
		p.fit_textline((LPCWSTR)str, wi2, hi, L"");
		hi-=fsize;

		break;
	default:
		break;
	}
	//p.fit_textline((LPCWSTR)resultStr, 20, hi, L"");


	hi-=fsize;


	//p.setfont(font, 18);
	str.LoadStringW(IDS_STRING_RESULT);
	p.fit_textline((LPCWSTR)str, wi, hi, L"");
	hi-=fsize;
	hi-=fsize;

	std::vector<CString> res;
	bool flg=Compute(dol, para, res);

	for(size_t i=0;i<res.size();i+=4){
		p.fit_textline((LPCWSTR)(res[i]), wi1, hi, L"");
		p.fit_textline((LPCWSTR)(res[i+1]), wi2, hi, L"");
		if(i+3<res.size()){
			p.fit_textline((LPCWSTR)(res[i+2]), wi3, hi, L"");
			p.fit_textline((LPCWSTR)(res[i+3]), wi4, hi, L"");
		}
		hi-=fsize;
	}

	hi-=fsize;

	str.LoadStringW(IDS_STRING_CV_PARA);
	p.fit_textline((LPCWSTR)str, wi, hi, L"");
	hi-=fsize;
	hi-=fsize;

	str.LoadStringW(IDS_STRING_LOW_E_LIMIT);
	p.fit_textline((LPCWSTR)str, wi1, hi, L"");

	str.Format(L"%g",p2.lowelimit);
	p.fit_textline((LPCWSTR)str, wi2, hi, L"");

	str.LoadStringW(IDS_STRING_HIGH_E_LIMIT);
	p.fit_textline((LPCWSTR)str, wi3, hi, L"");

	str.Format(L"%g",p2.highelimit);
	p.fit_textline((LPCWSTR)str, wi4, hi, L"");

	hi-=fsize;

	str.LoadStringW(IDS_STRING_SCAN_RATE);
	p.fit_textline((LPCWSTR)str, wi1, hi, L"");

	str.Format(L"%g",p2.scanrate);
	p.fit_textline((LPCWSTR)str, wi2, hi, L"");

	str.LoadStringW(IDS_STRING_NO_OF_CYCLES);
	p.fit_textline((LPCWSTR)str, wi3, hi, L"");

	str.Format(L"%d",p2.noofcycles);
	p.fit_textline((LPCWSTR)str, wi4, hi, L"");

	hi-=fsize;

	str.LoadStringW(IDS_STRING_ROTATION_RATE);
	p.fit_textline((LPCWSTR)str, wi1, hi, L"");

	str.Format(L"%g",p2.rotationrate);
	p.fit_textline((LPCWSTR)str, wi2, hi, L"");

	str.LoadStringW(IDS_STRING_END_INTEGRATION_E);
	p.fit_textline((LPCWSTR)str, wi3, hi, L"");

	str.Format(L"%g",p2.endintegratione);
	p.fit_textline((LPCWSTR)str, wi4, hi, L"");

	hi-=fsize;



	p.end_page_ext(L"");

	return 0;
}


void AddGI(pdflib::PDFlib &p, int & tbl, int & row, int & col, int colmax, int font)
{
	int fsize=10;
	int quartercolmax=colmax/4;
	std::wostringstream optlist;
	CString str;

	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font
		<< L" fontsize=" << fsize << L"}"
		//<< L" colwidth=25%"
		<< L" colspan=" << colmax
		;

	str.LoadStringW(IDS_STRING_GENERAL_INFO);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
	row++;




	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font
		<< L" fontsize=" << fsize << L"}"
		//<< L" colwidth=25%"
		<< L" colspan=" << quartercolmax
		;

	str.LoadStringW(IDS_STRING_SOFTWARE);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col+=quartercolmax;

	str.LoadStringW(AFX_IDS_APP_TITLE);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col+=quartercolmax;


	str.LoadStringW(IDS_STRING_TIME);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col+=quartercolmax;

	str=TimeString(true);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col=1;
	row++;
}

void AddANP(pdflib::PDFlib &p, int & tbl, int & row, int & col, int colmax, int font, const ANPara &para)
{

	int fsize=10;
	int quartercolmax=colmax/4;
	std::wostringstream optlist;
	CString str;


	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font		
		<< L" fontsize=" << fsize << L"}"
		<< L" colspan=" << colmax
		;
	//tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());

	//row++;

	str.LoadStringW(IDS_STRING_ANALYSIS_PARA);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	row++;

	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font
		<< L" fontsize=" << fsize << L"}"
		//<< L" colwidth=25%"
		<< L" colspan=" << quartercolmax
		;
	str.LoadStringW(IDS_STRING_ANALYSIS_TYPE);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col+=quartercolmax;

	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font		
		<< L" fontsize=" << fsize << L"}"
		<< L" colspan=" << colmax+1-col
		;
	str.LoadStringW(IDS_STRING_AT1+para.analysistype);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col=1;
	row++;


	switch(para.analysistype){
	case 3:
	case 11:
	case 4:
	case 6:
	case 10:
	case 12:
		break;

	default:

		optlist.str(L"");
		optlist << L"fittextline={position={left center} font=" << font		
			<< L" fontsize=" << fsize << L"}"
			<< L" colspan=" << quartercolmax
			//<< L" colwidth=25%"
			;
		str.LoadStringW(IDS_STRING_EVALUATION_RATIO);
		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

		col+=quartercolmax;

		optlist.str(L"");
		optlist << L"fittextline={position={left center} font=" << font		
			<< L" fontsize=" << fsize << L"}"
			<< L" colspan=" << colmax+1-col
			;
		str.Format(L"%g",para.evaluationratio);
		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


		col=1;
		row++;
		break;
	}

	switch(para.analysistype){
	case 2:
		{
			switch(para.calibrationfactortype){
			case 0:
				{


					optlist.str(L"");
					optlist << L"fittextline={position={left center} font=" << font		
						<< L" fontsize=" << fsize << L"}"
						<< L" colspan=" << quartercolmax
						//<< L" colwidth=25%"
						;
					str.LoadStringW(IDS_STRING_CALIBRATION_FACTOR);
					tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


					col+=quartercolmax;

					optlist.str(L"");
					optlist << L"fittextline={position={left center} font=" << font		
						<< L" fontsize=" << fsize << L"}"
						<< L" colspan=" << colmax+1-col
						;
					str.Format(L"%g",para.calibrationfactor);
					tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


					col=1;
					row++;


				}
				break;
			case 1:
				{


					optlist.str(L"");
					optlist << L"fittextline={position={left center} font=" << font		
						<< L" fontsize=" << fsize << L"}"
						<< L" colspan=" << quartercolmax
						//<< L" colwidth=25%"
						;
					str.LoadStringW(IDS_STRING_CALIBRATION_CURVE_FILE);
					tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


					col+=quartercolmax;

					optlist.str(L"");
					optlist << L"fittextline={position={left center} font=" << font		
						<< L" fontsize=" << fsize << L"}"
						<< L" colspan=" << colmax+1-col
						;
					//str.Format(L"%g",para.calibrationfactor);
					tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)para.calibrationfilepath, optlist.str());


					col=1;
					row++;


				}
				break;
			default:
				break;
			}
			break;
		}


	case 4:



		optlist.str(L"");
		optlist << L"fittextline={position={left center} font=" << font		
			<< L" fontsize=" << fsize << L"}"
			<< L" colspan=" << quartercolmax
			//<< L" colwidth=25%"
			;
		str.LoadStringW(IDS_STRING_INTERCEPT_VALUE);
		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


		col+=quartercolmax;

		optlist.str(L"");
		optlist << L"fittextline={position={left center} font=" << font		
			<< L" fontsize=" << fsize << L"}"
			<< L" colspan=" << colmax+1-col
			;
		str.Format(L"%g",para.interceptvalue);
		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


		col=1;
		row++;



		break;

	case 6:
	case 8:
	case 10:
	case 12:
		optlist.str(L"");
		optlist << L"fittextline={position={left center} font=" << font		
			<< L" fontsize=" << fsize << L"}"
			<< L" colspan=" << quartercolmax
			//<< L" colwidth=25%"
			;
		str.LoadStringW(IDS_STRING_CALIBRATION_CURVE_FILE);
		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


		col+=quartercolmax;

		optlist.str(L"");
		optlist << L"fittextline={position={left center} font=" << font		
			<< L" fontsize=" << fsize << L"}"
			<< L" colspan=" << colmax+1-col
			;
		//str.Format(L"%g",para.calibrationfactor);
		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)para.calibrationfilepath, optlist.str());


		col=1;
		row++;
		break;

	default:
		break;
	}
}


void AddCVP(pdflib::PDFlib &p, int & tbl, int & row, int & col, int colmax, int font, const CVPara &p2)
{

	int fsize=10;
	int quartercolmax=colmax/4;
	std::wostringstream optlist;
	CString str;

	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font		
		<< L" fontsize=" << fsize << L"}"
		<< L" colspan=" << colmax
		;
	str.LoadStringW(IDS_STRING_CV_PARA);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	row++;

	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font		
		<< L" fontsize=" << fsize << L"}"
		<< L" colspan=" << quartercolmax
		//<< L" colwidth=25%"
		;
	str.LoadStringW(IDS_STRING_LOW_E_LIMIT);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col+=quartercolmax;
	str.Format(L"%g",p2.lowelimit);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	col+=quartercolmax;
	str.LoadStringW(IDS_STRING_HIGH_E_LIMIT);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col+=quartercolmax;
	str.Format(L"%g",p2.highelimit);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col=1;
	row++;


	str.LoadStringW(IDS_STRING_SCAN_RATE);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col+=quartercolmax;
	str.Format(L"%g",p2.scanrate);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	col+=quartercolmax;
	str.LoadStringW(IDS_STRING_NO_OF_CYCLES);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col+=quartercolmax;
	str.Format(L"%d",p2.noofcycles);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());



	col=1;
	row++;


	str.LoadStringW(IDS_STRING_ROTATION_RATE);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col+=quartercolmax;
	str.Format(L"%g",p2.rotationrate);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	col+=quartercolmax;
	str.LoadStringW(IDS_STRING_END_INTEGRATION_E);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col+=quartercolmax;
	str.Format(L"%g",p2.endintegratione);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	col=1;
	row++;

}


void AddSAP(pdflib::PDFlib &p, int & tbl, int & row, int & col, int colmax, int font, const SAPara &p3)
{
	int fsize=10;
	//int quartercolmax=colmax/4;

	int seventhcolmax=(colmax/7)+1;
	int firstcol=colmax-6*seventhcolmax;

	std::wostringstream optlist;
	CString str;


	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font		
		<< L" fontsize=" << fsize << L"}"
		<< L" colspan=" << colmax
		;

	//tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());
	//row++;


	str.LoadStringW(IDS_STRING_ADDITION_SOLUTION_PARA);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	row++;

	int sarowstart=row;


	str.LoadStringW(IDS_STRING520);	
	optlist.str(L"");
	optlist << L"fittextline={position={center} font=" << font		
		<< L" fontsize=" << fsize << L"}"
		<< L" colspan=" << firstcol
		<< L" colwidth=10%"
		;
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
	col+=firstcol;

	//str.LoadStringW(IDS_STRING524);	
	//optlist.str(L"");
	//optlist << L"fittextline={position={center} font=" << font		
	//	<< L" fontsize=" << fsize << L"}"
	//	<< L" colspan=" << 3
	//	//<< L" colwidth=15%"
	//	;
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
	//col+=3;

	for(int i=1;i<=6;i++){
		str.LoadStringW(IDS_STRING520+i);	
		optlist.str(L"");
		optlist << L"fittextline={position={center} font=" << font		
			<< L" fontsize=" << fsize << L"}"
			<< L" colspan=" << seventhcolmax
			//<< L" colwidth=15%"
			;
		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
		col+=seventhcolmax;
	}

	//str.LoadStringW(IDS_STRING525);	
	//optlist.str(L"");
	//optlist << L"fittextline={position={center} font=" << font		
	//	<< L" fontsize=" << fsize << L"}"
	//	<< L" colspan=" << 3
	//	//<< L" colwidth=15%"
	//	;
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
	//col+=3;

	//str.LoadStringW(IDS_STRING526);	
	//optlist.str(L"");
	//optlist << L"fittextline={position={center} font=" << font		
	//	<< L" fontsize=" << fsize << L"}"
	//	<< L" colspan=" << 3
	//	//<< L" colwidth=15%"
	//	;
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
	//col+=3;

	for (row++; row <= sarowstart+p3.saplist.size(); row++) {

		col = 1;
		{
			std::wostringstream num;

			num << row-sarowstart-1;

			optlist.str(L"");
			optlist << L"fittextline={position={center} font=" << font		
				<< L" fontsize=" << fsize << L"}"
				<< L" colspan=" << firstcol
				<< L" colwidth=10%"
				;

			tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
			col += firstcol;
		}



		{
			str.LoadStringW(IDS_STRING_VOL_ONCE+p3.saplist[row-sarowstart-1].addType);

			optlist.str(L"");
			optlist << L"fittextline={position={center} font=" << font		
				<< L" fontsize=" << fsize << L"}"
				<< L" colspan=" << seventhcolmax
				//<< L" colwidth=15%"
				;

			tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(str), optlist.str());
			col+=seventhcolmax;
		}



		{
			//std::wostringstream num;


			str.Format(L"%g",p3.saplist[row-sarowstart-1].Sconc);
			//num << (LPCWSTR)(str);

			optlist.str(L"");
			optlist << L"fittextline={position={center} font=" << font		
				<< L" fontsize=" << fsize << L"}"
				<< L" colspan=" << seventhcolmax
				//<< L" colwidth=15%"
				;

			tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(str), optlist.str());
			col+=seventhcolmax;
		}



		{
			//std::wostringstream num;


			str.Format(L"%g",p3.saplist[row-sarowstart-1].Aconc);
			//num << (LPCWSTR)(str);

			optlist.str(L"");
			optlist << L"fittextline={position={center} font=" << font		
				<< L" fontsize=" << fsize << L"}"
				<< L" colspan=" << seventhcolmax
				//<< L" colwidth=15%"
				;

			tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(str), optlist.str());
			col+=seventhcolmax;
		}



		{
			//std::wostringstream num;


			str.Format(L"%g",p3.saplist[row-sarowstart-1].Lconc);
			//num << (LPCWSTR)(str);

			optlist.str(L"");
			optlist << L"fittextline={position={center} font=" << font		
				<< L" fontsize=" << fsize << L"}"
				<< L" colspan=" << seventhcolmax
				//<< L" colwidth=15%"
				;
			tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(str), optlist.str());
			col+=seventhcolmax;
		}



		{
			//std::wostringstream num;

			str.Format(L"%g",p3.saplist[row-sarowstart-1].volconc);
			//num << (LPCWSTR)(str);

			optlist.str(L"");
			optlist << L"fittextline={position={center} font=" << font		
				<< L" fontsize=" << fsize << L"}"
				<< L" colspan=" << seventhcolmax
				//<< L" colwidth=15%"
				;

			tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(str), optlist.str());
			col+=seventhcolmax;
		}


		{
			//std::wostringstream num;


			str.Format(L"%g",p3.saplist[row-sarowstart-1].endRatio);
			//num << (LPCWSTR)(str);

			optlist.str(L"");
			optlist << L"fittextline={position={center} font=" << font		
				<< L" fontsize=" << fsize << L"}"
				<< L" colspan=" << seventhcolmax
				//<< L" colwidth=15%"
				;

			tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(str), optlist.str());
		}


	}

	//int sarowend=row;
	col=1;
}


void AddRES(pdflib::PDFlib &p, int & tbl, int & row, int & col, int colmax, int font, const std::vector<CString> &res)
{

	int fsize=10;
	int quartercolmax=colmax/4;

	//int seventhcolmax=(colmax/7)+1;
	//int firstcol=colmax-6*seventhcolmax;

	std::wostringstream optlist;
	CString str;

	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font		
		<< L" fontsize=" << fsize << L"}"
		<< L" colspan=" << colmax
		;
	//tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());

	//row++;

	str.LoadStringW(IDS_STRING_RESULT);

	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	row++;

	for(size_t i=0;i<res.size();i+=2){


		optlist.str(L"");
		optlist << L"fittextline={position={left center} font=" << font		
			<< L" fontsize=" << fsize << L"}"
			<< L" colspan=" << quartercolmax
			//<< L" colwidth=25%"
			;

		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(res[i]), optlist.str());


		col+=quartercolmax;

		optlist.str(L"");
		optlist << L"fittextline={position={left center} font=" << font		
			<< L" fontsize=" << fsize << L"}"
			<< L" colspan=" << colmax+1-col
			;
		//str.Format(L"%g",para.calibrationfactor);
		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(res[i+1]), optlist.str());


		col=1;
		row++;
	}

}


int pdfout6(
	pdflib::PDFlib &p, 
	const ANPara &para, 
	const std::vector<CString> &res, 
	const CVPara &p2, 
	const SAPara &p3,
	bool b1,
	bool b2,
	bool b3,
	bool b4
	){


	int titlesize=16;

	//std::vector<CString> res;
	//bool flg=Compute(dol, para, res);

	//int font;
	//font = p.load_font(L"Helvetica", L"winansi", L"");
	//if (font == -1) {
	//	std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
	//	return 2;
	//}

	/* Start page 1 */
	//p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");
	//p.setfont(font, fsize);




	int row, col, font, image, tf=-1, tbl=-1;
	int rowmax=1+2+1+4+1+1+res.size()+1+4, colmax=20;


	//int quartercolmax=colmax/4;

	//int rowmax=para.saplist.size()+2, colmax=7;

	//PDFlib p;
	double llx=50, lly=50, urx=550, ury=800;
	int fsize=10;
	CString str;



	//L"Table header (centered across all columns)";
	std::wstring result;
	std::wostringstream optlist;

	/* -------------------- Add table cells -------------------- */
	row = 1; col = 1;
	/* ---------- Row 1: table header (spans all columns) */



	//const std::wstring imagefile = L"C:\\Users\\r8anw2x\\Documents\\Visual Studio 2010\\project0\\analyzer\\analyzer\\res\\sinyang.bmp";

	image = p.load_image(L"auto", logofile, L"");
	if (image == -1) {
		std::wcerr << L"Couldn't load image: " << p.get_errmsg() << std::endl;
		return 2;
	}

	optlist.str(L"");
	optlist << L"image=" << image << L" fitimage={fitmethod=auto position={right center} }";
	optlist << L" rowheight=30";
	optlist << L" colspan=" << colmax;

	tbl = p.add_table_cell(tbl, col, row, L"", optlist.str());
	if (tbl == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}

	row++;

	font = p.load_font(L"Helvetica", L"winansi", L"");
	if (font == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}

	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font
		<< L" fontsize=" << fsize << L"}"
		//<< L" colwidth=25%"
		<< L" colspan=" << colmax
		;

	tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());
	row++;

	/* ---------- Row 1: table header (spans all columns) */
	font = p.load_font(L"Times-Bold", L"winansi", L"");
	if (font == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return(2);
	}

	optlist.str(L"");
	optlist << L"fittextline={position=center font=" << font
		<< L" fontsize=" << titlesize << L"}"
		<< L" colwidth=100%"
		<< L" colspan=" << colmax
		;

	str.LoadStringW(IDS_STRING_REPORT);
	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
	row++;


	/* ---------- Row 1: table header (spans all columns) */
	font = p.load_font(L"Helvetica", L"winansi", L"");
	if (font == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}

	AddGI(p,tbl,row,col,colmax,font);


	//optlist.str(L"");
	//optlist << L"fittextline={position={left center} font=" << font
	//	<< L" fontsize=" << fsize << L"}"
	//	//<< L" colwidth=25%"
	//	<< L" colspan=" << colmax
	//	;

	//str.LoadStringW(IDS_STRING_GENERAL_INFO);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
	//row++;




	//optlist.str(L"");
	//optlist << L"fittextline={position={left center} font=" << font
	//	<< L" fontsize=" << fsize << L"}"
	//	//<< L" colwidth=25%"
	//	<< L" colspan=" << quartercolmax
	//	;

	//str.LoadStringW(IDS_STRING_SOFTWARE);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col+=quartercolmax;

	//str.LoadStringW(AFX_IDS_APP_TITLE);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col+=quartercolmax;


	//str.LoadStringW(IDS_STRING_TIME);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col+=quartercolmax;

	//str=TimeString(true);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col=1;
	//row++;

	/* ---------- Row 1: table header (spans all columns) */


	if(b1){

	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font		
		<< L" fontsize=" << fsize << L"}"
		<< L" colspan=" << colmax
		;
	tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());

	row++;

	AddANP(p,tbl,row,col,colmax,font,para);

	}
	//str.LoadStringW(IDS_STRING_ANALYSIS_PARA);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//row++;

	//optlist.str(L"");
	//optlist << L"fittextline={position={left center} font=" << font
	//	<< L" fontsize=" << fsize << L"}"
	//	//<< L" colwidth=25%"
	//	<< L" colspan=" << quartercolmax
	//	;
	//str.LoadStringW(IDS_STRING_ANALYSIS_TYPE);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col+=quartercolmax;

	//optlist.str(L"");
	//optlist << L"fittextline={position={left center} font=" << font		
	//	<< L" fontsize=" << fsize << L"}"
	//	<< L" colspan=" << colmax+1-col
	//	;
	//str.LoadStringW(IDS_STRING_AT1+para.analysistype);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col=1;
	//row++;

	//optlist.str(L"");
	//optlist << L"fittextline={position={left center} font=" << font		
	//	<< L" fontsize=" << fsize << L"}"
	//	<< L" colspan=" << quartercolmax
	//	//<< L" colwidth=25%"
	//	;
	//str.LoadStringW(IDS_STRING_EVALUATION_RATIO);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col+=quartercolmax;

	//optlist.str(L"");
	//optlist << L"fittextline={position={left center} font=" << font		
	//	<< L" fontsize=" << fsize << L"}"
	//	<< L" colspan=" << colmax+1-col
	//	;
	//str.Format(L"%g",para.evaluationratio);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	//col=1;
	//row++;

	//switch(para.analysistype){
	//case 2:
	//	{
	//		switch(para.calibrationfactortype){
	//		case 0:
	//			{


	//				optlist.str(L"");
	//				optlist << L"fittextline={position={left center} font=" << font		
	//					<< L" fontsize=" << fsize << L"}"
	//					<< L" colspan=" << quartercolmax
	//					//<< L" colwidth=25%"
	//					;
	//				str.LoadStringW(IDS_STRING_CALIBRATION_FACTOR);
	//				tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	//				col+=quartercolmax;

	//				optlist.str(L"");
	//				optlist << L"fittextline={position={left center} font=" << font		
	//					<< L" fontsize=" << fsize << L"}"
	//					<< L" colspan=" << colmax+1-col
	//					;
	//				str.Format(L"%g",para.calibrationfactor);
	//				tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	//				col=1;
	//				row++;


	//			}
	//			break;
	//		case 1:
	//			{


	//				optlist.str(L"");
	//				optlist << L"fittextline={position={left center} font=" << font		
	//					<< L" fontsize=" << fsize << L"}"
	//					<< L" colspan=" << quartercolmax
	//					//<< L" colwidth=25%"
	//					;
	//				str.LoadStringW(IDS_STRING_CALIBRATION_CURVE_FILE);
	//				tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	//				col+=quartercolmax;

	//				optlist.str(L"");
	//				optlist << L"fittextline={position={left center} font=" << font		
	//					<< L" fontsize=" << fsize << L"}"
	//					<< L" colspan=" << colmax+1-col
	//					;
	//				//str.Format(L"%g",para.calibrationfactor);
	//				tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)para.calibrationfilepath, optlist.str());


	//				col=1;
	//				row++;


	//			}
	//			break;
	//		default:
	//			break;
	//		}
	//		break;
	//	}

	//case 6:
	//case 8:
	//case 10:
	//case 12:
	//	optlist.str(L"");
	//	optlist << L"fittextline={position={left center} font=" << font		
	//		<< L" fontsize=" << fsize << L"}"
	//		<< L" colspan=" << quartercolmax
	//		//<< L" colwidth=25%"
	//		;
	//	str.LoadStringW(IDS_STRING_CALIBRATION_CURVE_FILE);
	//	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	//	col+=quartercolmax;

	//	optlist.str(L"");
	//	optlist << L"fittextline={position={left center} font=" << font		
	//		<< L" fontsize=" << fsize << L"}"
	//		<< L" colspan=" << colmax+1-col
	//		;
	//	//str.Format(L"%g",para.calibrationfactor);
	//	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)para.calibrationfilepath, optlist.str());


	//	col=1;
	//	row++;
	//	break;
	//case 4:



	//	optlist.str(L"");
	//	optlist << L"fittextline={position={left center} font=" << font		
	//		<< L" fontsize=" << fsize << L"}"
	//		<< L" colspan=" << quartercolmax
	//		//<< L" colwidth=25%"
	//		;
	//	str.LoadStringW(IDS_STRING_INTERCEPT_VALUE);
	//	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	//	col+=quartercolmax;

	//	optlist.str(L"");
	//	optlist << L"fittextline={position={left center} font=" << font		
	//		<< L" fontsize=" << fsize << L"}"
	//		<< L" colspan=" << colmax+1-col
	//		;
	//	str.Format(L"%g",para.interceptvalue);
	//	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	//	col=1;
	//	row++;



	//	break;
	//default:
	//	break;
	//}

	/* ---------- Row 1: table header (spans all columns) */

if(b2){
	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font		
		<< L" fontsize=" << fsize << L"}"
		<< L" colspan=" << colmax
		;
	tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());

	row++;

	AddCVP(p,tbl,row,col,colmax,font,p2);
}
	//optlist.str(L"");
	//optlist << L"fittextline={position={left center} font=" << font		
	//	<< L" fontsize=" << fsize << L"}"
	//	<< L" colspan=" << colmax
	//	;
	//str.LoadStringW(IDS_STRING_CV_PARA);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	//row++;

	//optlist.str(L"");
	//optlist << L"fittextline={position={left center} font=" << font		
	//	<< L" fontsize=" << fsize << L"}"
	//	<< L" colspan=" << quartercolmax
	//	//<< L" colwidth=25%"
	//	;
	//str.LoadStringW(IDS_STRING_LOW_E_LIMIT);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col+=quartercolmax;
	//str.Format(L"%g",p2.lowelimit);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	//col+=quartercolmax;
	//str.LoadStringW(IDS_STRING_HIGH_E_LIMIT);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col+=quartercolmax;
	//str.Format(L"%g",p2.highelimit);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col=1;
	//row++;


	//str.LoadStringW(IDS_STRING_SCAN_RATE);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col+=quartercolmax;
	//str.Format(L"%g",p2.scanrate);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	//col+=quartercolmax;
	//str.LoadStringW(IDS_STRING_NO_OF_CYCLES);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col+=quartercolmax;
	//str.Format(L"%d",p2.noofcycles);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());



	//col=1;
	//row++;


	//str.LoadStringW(IDS_STRING_ROTATION_RATE);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col+=quartercolmax;
	//str.Format(L"%g",p2.rotationrate);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	//col+=quartercolmax;
	//str.LoadStringW(IDS_STRING_END_INTEGRATION_E);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col+=quartercolmax;
	//str.Format(L"%g",p2.endintegratione);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//col=1;
	//row++;





	/* ---------- Row 1: table header (spans all columns) */
if(b3){
	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font		
		<< L" fontsize=" << fsize << L"}"
		<< L" colspan=" << colmax
		;

	tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());
	row++;


	//str.LoadStringW(IDS_STRING_ADDITION_SOLUTION_PARA);
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());

	//row++;

	//int sarowstart=row;

	AddSAP(p,tbl,row,col,colmax,font,p3);
}
	//str.LoadStringW(IDS_STRING520);	
	//optlist.str(L"");
	//optlist << L"fittextline={position={center} font=" << font		
	//	<< L" fontsize=" << fsize << L"}"
	//	<< L" colspan=" << 2
	//	<< L" colwidth=10%"
	//	;
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
	//col+=2;

	//str.LoadStringW(IDS_STRING524);	
	//optlist.str(L"");
	//optlist << L"fittextline={position={center} font=" << font		
	//	<< L" fontsize=" << fsize << L"}"
	//	<< L" colspan=" << 3
	//	//<< L" colwidth=15%"
	//	;
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
	//col+=3;

	//for(int i=1;i<=3;i++){
	//	str.LoadStringW(IDS_STRING524+i);	
	//	optlist.str(L"");
	//	optlist << L"fittextline={position={center} font=" << font		
	//		<< L" fontsize=" << fsize << L"}"
	//		<< L" colspan=" << 3
	//		//<< L" colwidth=15%"
	//		;
	//	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
	//	col+=3;
	//}

	//str.LoadStringW(IDS_STRING525);	
	//optlist.str(L"");
	//optlist << L"fittextline={position={center} font=" << font		
	//	<< L" fontsize=" << fsize << L"}"
	//	<< L" colspan=" << 3
	//	//<< L" colwidth=15%"
	//	;
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
	//col+=3;

	//str.LoadStringW(IDS_STRING526);	
	//optlist.str(L"");
	//optlist << L"fittextline={position={center} font=" << font		
	//	<< L" fontsize=" << fsize << L"}"
	//	<< L" colspan=" << 3
	//	//<< L" colwidth=15%"
	//	;
	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
	//col+=3;

	//for (row++; row <= sarowstart+p3.saplist.size(); row++) {

	//	col = 1;
	//	{
	//		std::wostringstream num;

	//		num << row-sarowstart-1;

	//		optlist.str(L"");
	//		optlist << L"fittextline={position={center} font=" << font		
	//			<< L" fontsize=" << fsize << L"}"
	//			<< L" colspan=" << 2
	//			<< L" colwidth=10%"
	//			;

	//		tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
	//		col += 2;
	//	}



	//	{
	//		str.LoadStringW(IDS_STRING_VOL_ONCE+p3.saplist[row-sarowstart-1].addType);

	//		optlist.str(L"");
	//		optlist << L"fittextline={position={center} font=" << font		
	//			<< L" fontsize=" << fsize << L"}"
	//			<< L" colspan=" << 3
	//			//<< L" colwidth=15%"
	//			;

	//		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(str), optlist.str());
	//		col+=3;
	//	}



	//	{
	//		//std::wostringstream num;


	//		str.Format(L"%g",p3.saplist[row-sarowstart-1].Sconc);
	//		//num << (LPCWSTR)(str);

	//		optlist.str(L"");
	//		optlist << L"fittextline={position={center} font=" << font		
	//			<< L" fontsize=" << fsize << L"}"
	//			<< L" colspan=" << 3
	//			//<< L" colwidth=15%"
	//			;

	//		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(str), optlist.str());
	//		col+=3;
	//	}



	//	{
	//		//std::wostringstream num;


	//		str.Format(L"%g",p3.saplist[row-sarowstart-1].Aconc);
	//		//num << (LPCWSTR)(str);

	//		optlist.str(L"");
	//		optlist << L"fittextline={position={center} font=" << font		
	//			<< L" fontsize=" << fsize << L"}"
	//			<< L" colspan=" << 3
	//			//<< L" colwidth=15%"
	//			;

	//		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(str), optlist.str());
	//		col+=3;
	//	}



	//	{
	//		//std::wostringstream num;


	//		str.Format(L"%g",p3.saplist[row-sarowstart-1].Lconc);
	//		//num << (LPCWSTR)(str);

	//		optlist.str(L"");
	//		optlist << L"fittextline={position={center} font=" << font		
	//			<< L" fontsize=" << fsize << L"}"
	//			<< L" colspan=" << 3
	//			//<< L" colwidth=15%"
	//			;
	//		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(str), optlist.str());
	//		col+=3;
	//	}



	//	{
	//		//std::wostringstream num;

	//		str.Format(L"%g",p3.saplist[row-sarowstart-1].volconc);
	//		//num << (LPCWSTR)(str);

	//		optlist.str(L"");
	//		optlist << L"fittextline={position={center} font=" << font		
	//			<< L" fontsize=" << fsize << L"}"
	//			<< L" colspan=" << 3
	//			//<< L" colwidth=15%"
	//			;

	//		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(str), optlist.str());
	//		col+=3;
	//	}


	//	{
	//		//std::wostringstream num;


	//		str.Format(L"%g",p3.saplist[row-sarowstart-1].endRatio);
	//		//num << (LPCWSTR)(str);

	//		optlist.str(L"");
	//		optlist << L"fittextline={position={center} font=" << font		
	//			<< L" fontsize=" << fsize << L"}"
	//			<< L" colspan=" << 3
	//			//<< L" colwidth=15%"
	//			;

	//		tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(str), optlist.str());
	//	}


	//}

	int sarowend=row;
	//col=1;

	/* ---------- Row 1: table header (spans all columns) */
	if(b4){
	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font		
		<< L" fontsize=" << fsize << L"}"
		<< L" colspan=" << colmax
		;
	tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());

	row++;


	AddRES(p,tbl,row,col,colmax,font,res);
	}

	//str.LoadStringW(IDS_STRING_RESULT);

	//tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());


	//row++;

	//for(size_t i=0;i<res.size();i+=2){


	//	optlist.str(L"");
	//	optlist << L"fittextline={position={left center} font=" << font		
	//		<< L" fontsize=" << fsize << L"}"
	//		<< L" colspan=" << quartercolmax
	//		//<< L" colwidth=25%"
	//		;

	//	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(res[i]), optlist.str());


	//	col+=quartercolmax;

	//	optlist.str(L"");
	//	optlist << L"fittextline={position={left center} font=" << font		
	//		<< L" fontsize=" << fsize << L"}"
	//		<< L" colspan=" << colmax+1-col
	//		;
	//	//str.Format(L"%g",para.calibrationfactor);
	//	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(res[i+1]), optlist.str());


	//	col=1;
	//	row++;
	//}






	//optlist.str(L"");
	//optlist <<L"fill={";
	//for(int i=sarowstart;i<sarowend;i+=2){
	//	optlist << L"{area=row"
	//		<< i
	//		<< L" fillcolor={gray 0.9}} ";
	//}
	//optlist << L"} "; 
	//optlist << L"stroke={{line=other}} ";


	/* Place the table instance */
	//result = p.fit_table(tbl, llx, lly, urx, ury, optlist.str());

	////result = p.fit_table(tbl, llx, lly, urx, ury, L"fill={{area=row20 fillcolor={gray 0.9}} {area=row22 fillcolor={gray 0.9}} {area=row24 fillcolor={gray 0.9}}}" );
	//if (result == L"_error") {
	//	std::wcerr << L"Couldn't place table: " << p.get_errmsg() << std::endl;
	//	return 2;
	//}









	//p.end_page_ext(L"");








	optlist.str(L"");
	if(b3){
	optlist <<L"fill={";
	for(int i=sarowend-p3.saplist.size()-1;i<sarowend;i+=2){
		optlist << L"{area=row"
			<< i
			<< L" fillcolor={gray 0.9}} ";
	}
	optlist << L"} "; 
	//optlist << L"stroke={{line=other}} ";
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




		/* Place the table instance */
		result = p.fit_table(tbl, llx, lly, urx, ury, optlist.str());


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

	//return 0;







	return 0;
}





int imgout(pdflib::PDFlib &p, const std::wstring imagefile, CString str)
{

	int fsize=10;

	//int font, image;

	std::wstring result;
	std::wostringstream optlist;

	double llx=50, lly=50, urx=550, ury=800;

	int row, col, font, image, tf=-1, tbl=-1;

	//int font;
	//font = p.load_font(L"Helvetica", L"winansi", L"");
	//if (font == -1) {
	//	std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
	//	return 2;
	//}


	p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

	/* Position the image in its original size with its lower left corner
	* at the reference point (20, 380)
	*/



	/* -------------------- Add table cells -------------------- */
	row = 1; col = 1;
	/* ---------- Row 1: table header (spans all columns) */





	image = p.load_image(L"auto", logofile, L"");
	if (image == -1) {
		std::wcerr << L"Couldn't load image: " << p.get_errmsg() << std::endl;
		return 2;
	}

	optlist.str(L"");
	optlist << L"image=" << image << L" fitimage={fitmethod=auto position={right center}}";
	optlist << L" rowheight=30";
	//optlist << L" colspan=" << colmax;

	tbl = p.add_table_cell(tbl, col, row, L"", optlist.str());
	if (tbl == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}

	row++;


	font = p.load_font(L"Helvetica", L"winansi", L"");
	if (font == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}

	optlist.str(L"");
	optlist << L"fittextline={position={left center} font=" << font
		<< L" fontsize=" << fsize << L"}"
		<< L" colwidth=100%"
		//<< L" colspan=" << colmax
		;

	tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());
	row++;

	tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)str, optlist.str());
	row++;



	image = p.load_image(L"auto", imagefile, L"");
	if (image == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}

	optlist.str(L"");
	optlist << L"image=" << image << L" fitimage={fitmethod=auto position={left top}}";
	optlist << L" rowheight=600";
	//optlist << L" colspan=" << colmax;

	tbl = p.add_table_cell(tbl, col, row, L"", optlist.str());
	if (tbl == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}

	row++;




	//p.setfont(font, fsize);

	//p.fit_textline((LPCWSTR)str, 50, 760, L"");


	//optlist.str(L"");
	//optlist << L"boxsize={500 700}"
	//	<< L" fitmethod=auto"
	//	<< L" position={left top}";

	//p.fit_image(image, 50, 50, optlist.str());


	optlist.str(L"");
	//optlist <<L"fill={";
	//for(int i=sarowstart;i<sarowend;i+=2){
	//	optlist << L"{area=row"
	//		<< i
	//		<< L" fillcolor={gray 0.9}} ";
	//}
	//optlist << L"} "; 
	//optlist << L"stroke={{line=other}} ";


	/* Place the table instance */
	result = p.fit_table(tbl, llx, lly, urx, ury, optlist.str());

	//result = p.fit_table(tbl, llx, lly, urx, ury, L"fill={{area=row20 fillcolor={gray 0.9}} {area=row22 fillcolor={gray 0.9}} {area=row24 fillcolor={gray 0.9}}}" );
	if (result == L"_error") {
		std::wcerr << L"Couldn't place table: " << p.get_errmsg() << std::endl;
		return 2;
	}




	p.end_page_ext(L"");

	return 0;
}





int imgout2(
	pdflib::PDFlib &p, 
	//CanalyzerDoc *padoc, 
	CDC *pdc,
	std::vector<PlotData> &imagelist, 
	std::vector<CString> &namelist, 
	CSize szimg)
{



	int fsize=10;

	//const std::wstring searchpath = L"../data";

	int row, col, font, image, tf=-1, tbl=-1;
	//int rowmax=dol.size()+2, colmax=7;
	//PDFlib p;
	double llx=50, lly=50, urx=550, ury=800;

	//CString str;
	CString fptmp=L"temp.bmp";

	//str.LoadStringW(IDS_OUTPUT_WND);

	//const std::wstring headertext = (LPCWSTR)str;
	//L"Table header (centered across all columns)";
	std::wstring result;
	std::wostringstream optlist;

	/* -------------------- Add table cells -------------------- */
	row = 1; col = 1;
	/* ---------- Row 1: table header (spans all columns) */





	//const std::wstring imagefile = L"C:\\Users\\r8anw2x\\Documents\\Visual Studio 2010\\project0\\analyzer\\analyzer\\res\\sinyang.bmp";

	image = p.load_image(L"auto", logofile, L"");
	if (image == -1) {
		std::wcerr << L"Couldn't load image: " << p.get_errmsg() << std::endl;
		return 2;
	}

	optlist.str(L"");
	optlist << L"image=" << image << L" fitimage={fitmethod=auto position={right center}}";
	optlist << L" rowheight=30";
	//optlist << L" colspan=" << colmax;
	optlist << L" colwidth=100%";

	tbl = p.add_table_cell(tbl, col, row, L"", optlist.str());
	if (tbl == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return 2;
	}

	row++;




	font = p.load_font(L"Helvetica", L"winansi", L"");
	if (font == -1) {
		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
		return(2);
	}


	while( !namelist.empty()
		&& !imagelist.empty() ){

			optlist.str(L"");
			optlist << L"fittextline={position={left center} font=" << font
				<< L" fontsize=" << fsize 
				<< L"}"
				//<< L" colspan=" << colmax
				;
			optlist << L" rowheight=15";



			tbl = p.add_table_cell(tbl, col, row, (LPCWSTR)(namelist.back()), optlist.str());
			row++;

			//tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());
			//row++;

			//std::wstring imagefile

			//padoc->SaveImagePrint(imagelist.back(),szimg,fptmp);

			//padoc->SaveImage(imagelist.back(),szimg,fptmp);

			SaveImage(imagelist.back(), szimg, fptmp, pdc);

			image = p.load_image(L"auto", (LPCWSTR)fptmp, L"");
			if (image == -1) {
				std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
				return 2;
			}

			optlist.str(L"");
			optlist << L"image=" << image << L" fitimage={fitmethod=auto position={center top}}";
			optlist << L" rowheight=345";
			//optlist << L" colspan=" << colmax;

			tbl = p.add_table_cell(tbl, col, row, L"", optlist.str());
			if (tbl == -1) {
				std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
				return 2;
			}

			row++;

			namelist.pop_back();
			imagelist.pop_back();

	}


	optlist.str(L"");
	//optlist <<L"fill={";
	//optlist << L"{area=roweven"
	//<< L" fillcolor={gray 0.918}} ";

	//for(int i=rowstart;i<rowend;i+=2){
	//optlist << L"{area=row"
	//<< i
	//<< L" fillcolor={gray 0.9}} ";
	//}

	//optlist << L"} "; 
	//optlist << L"stroke={{line=other}} ";
	optlist << L"header=1 ";



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
		result = p.fit_table(tbl, llx, lly, urx, ury, optlist.str());
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


	CFile::Remove(fptmp);

	return 0;




}


//
//
//int pdfd(CString outfile, CanalyzerDoc *padoc)
//{
//	const std::wstring searchpath = L"../data";
//
//	const std::wstring temppdf = L"temp.pdf";
//
//	//CSize szimg(800,600);
//	//CSize szimg(1200,900);
//	//CSize szimg(600,450);
//
//	//CString fptmp=L"temp.bmp";
//
//
//	pdflib::PDFlib p;
//
//	std::wostringstream optlist;
//
//	optlist.str(L"");
//	//optlist << L"labels={pagenumber=1}";
//
//
//	p.set_parameter(L"errorpolicy", L"return");
//
//	p.set_parameter(L"SearchPath", searchpath);
//
//	if (p.begin_document(temppdf, optlist.str()) == -1) {
//		//if (p.begin_document((LPCWSTR)outfile, L"") == -1) {
//		std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
//		return 2;
//	}
//	p.set_info(L"Creator", L"PDFlib starter sample");
//	p.set_info(L"Title", L"starter_table");
//
//	int a;
//
//
//	std::vector<CString> res;
//	bool flg=Compute(padoc->dol, padoc->p1, res);
//
//	//a=pdfout4(p,padoc->p1,padoc->dol);
//	a=pdfout6(p,padoc->p1,res,padoc->p2,padoc->p3);
//	//a=pdfout3(p,padoc->p1,padoc->resultStr);
//
//	//a=pdfout1(p,padoc->p3);
//	//a=pdfout2(p,padoc->p2);
//	a=pdfout(p,padoc->dol);
//
//
//
//
//	std::vector<PlotData> pdl;
//	pdl.assign(padoc->lp.begin(),padoc->lp.end());
//	pdl.resize(padoc->rp.size()+padoc->lp.size());
//	std::copy_backward(padoc->rp.begin(),padoc->rp.end(),pdl.end());
//
//	CString str;
//	str.LoadStringW(IDS_STRING_VOLTAMMOGRAM);
//	std::vector<CString> nl;
//	nl.assign(padoc->lp.size(),str);
//	str.LoadStringW(IDS_STRING_TEST_CURVE);
//	nl.resize(padoc->rp.size()+padoc->lp.size(),str);
//	
//
//	a=imgout2(p,padoc,pdl,nl);
//
//
//
//
//	//for(size_t i=0;i<padoc->rp.size();i++){
//	//	CString str;
//	//	str.LoadStringW(IDS_STRING_TEST_CURVE);
//
//	//	padoc->SaveImage(padoc->rp[i],szimg,fptmp);
//	//	imgout(p,(LPCWSTR)fptmp,str);
//	//}
//
//	//for(size_t i=0;i<padoc->lp.size();i++){
//	//	CString str;
//	//	str.LoadStringW(IDS_STRING_VOLTAMMOGRAM);
//
//	//	padoc->SaveImage(padoc->lp[i],szimg,fptmp);
//	//	imgout(p,(LPCWSTR)fptmp,str);
//	//}
//
//
//
//	//CFile::Remove(fptmp);
//
//	//p.end_document(L"");
//
//	p.end_document(optlist.str());
//
//	AddPageNumber(temppdf,(LPCWSTR)outfile);
//
//	CFile::Remove(temppdf.c_str());
//
//	return 0;
//}
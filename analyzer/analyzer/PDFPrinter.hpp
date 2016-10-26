#pragma once

#include <afxwin.h>
#include <atltypes.h>
#include <vector>


#include "p/pdflib.hpp"
#pragma comment(lib, "p/pdflib.lib")

#include "struct\DocDataEx.hpp"

#include "struct1\PlotDataEx.hpp"

#include <iostream>


class PDFPrinter
{
public:
	static CString TimeString(bool bSeg=false)
	{
		const int l=20;
		char buffer [l];
		time_t rawtime;
		time (&rawtime);
		struct tm * timeinfo = localtime (&rawtime);
		if(bSeg){
			strftime (buffer,l,"%Y-%m-%d %H:%M:%S",timeinfo);
		}
		else{
			strftime (buffer,l,"%Y%m%d%H%M%S",timeinfo);
		}

		//puts (buffer);
		wchar_t buf[l];
		size_t aa=mbstowcs(buf,buffer,l);
		return CString(buf);
	};



	static int AddPageNumber(const std::wstring fin, const std::wstring fout)
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
			//strt<<L"page "<<pageno<<L" of "<<endpage;
			strt<<pageno<<L"/"<<endpage;
			p.fit_textline(strt.str(), 20, 20, L"");

			p.end_page_ext(L"");
		}
		p.close_pdi_document(indoc);


		p.end_document(L"");

		return 0;
	}



	static int pdfout(pdflib::PDFlib &p, 
		const std::vector<DataOutA> &dol,
		const std::wstring logofile){

			//const std::wstring searchpath = L"../data";

			int row, col, font, image, tf=-1, tbl=-1;
			int rowmax=dol.size()+2, colmax=7;
			//PDFlib p;
			double llx=50, lly=50, urx=550, ury=800;
			int titlefont,fsize=10,titlesize=12;
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
			optlist << L"image=" << image << L" fitimage={fitmethod=auto position={right center}}";
			optlist << L" rowheight=30";
			optlist << L" colspan=" << colmax;

			tbl = p.add_table_cell(tbl, col, row, L"", optlist.str());
			if (tbl == -1) {
				std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
				return 2;
			}

			row++;


			str.LoadStringW(IDS_OUTPUT_WND);
			const std::wstring headertext = (LPCWSTR)str;

			titlefont = p.load_font(L"Times-Bold", L"winansi", L"");
			if (titlefont == -1) {
				std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
				return(2);
			}

			optlist.str(L"");
			optlist << L"fittextline={position={left center} font=" << titlefont
				<< L" fontsize="<< titlesize << L"}"
				<< L" colspan=" << colmax;

			tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());
			row++;

			tbl = p.add_table_cell(tbl, col, row, headertext, optlist.str());
			row++;


			font = p.load_font(L"Helvetica", L"winansi", L"");
			if (font == -1) {
				std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
				return(2);
			}

			int rowstart=row;

			for(col = 1;col<=7;col++)
			{
				std::wostringstream num;

				//num << L"Col " << col << L"/Row " << row;

				str.LoadStringW(IDS_STRING111+col-1);

				num << (LPCWSTR)str;

				optlist.str(L"");
				optlist << L"colwidth=20% fittextline={font=" << font
					<< L" fontsize="<< fsize << L"}";

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
						<< L" fontsize="<< fsize << L"}";

					tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
				}


				col = 2;
				{
					std::wostringstream num;

					//num << L"Col " << col << L"/Row " << row;

					num << /*(LPWSTR)_bstr_t*/(LPCWSTR)(doa.GetStepName(di));

					optlist.str(L"");
					optlist << L"colwidth=15% fittextline={font=" << font
						<< L" fontsize="<< fsize << L"}";

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
						<< L" fontsize="<< fsize << L"}";

					tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
				}


				col = 4;
				{
					std::wostringstream num;

					//num << L"Col " << col << L"/Row " << row;

					num << doa.TotalVolume();
					optlist.str(L"");
					optlist << L"colwidth=15% fittextline={font=" << font
						<< L" fontsize="<< fsize << L"}";

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
						<< L" fontsize="<< fsize << L"}";

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
						<< L" fontsize="<< fsize << L"}";

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
						<< L" fontsize="<< fsize << L"}";

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
	};


	static void AddGI(pdflib::PDFlib &p, int & tbl, int & row, int & col, int colmax, int font, int titlefont, int fsize=10, int titlesize=12)
	{
		//int fsize=10;
		int quartercolmax=colmax/4;
		std::wostringstream optlist;
		CString str;

		optlist.str(L"");
		optlist << L"fittextline={position={left center} font=" << titlefont
			<< L" fontsize=" << titlesize << L"}"
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
	};

	static void AddANP(pdflib::PDFlib &p, int & tbl, int & row, int & col, int colmax, int font, int titlefont, const ANPara &para, int fsize=10, int titlesize=12)
	{

		//int fsize=10;
		int quartercolmax=colmax/4;
		std::wostringstream optlist;
		CString str;


		optlist.str(L"");
		optlist << L"fittextline={position={left center} font=" << titlefont		
			<< L" fontsize=" << titlesize << L"}"
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
						str.LoadStringW(IDS_EDIT_CALIBRATION_FACTOR);
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
			str.LoadStringW(IDS_EDIT_INTERCEPT_VALUE);
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
	};


	static void AddCVP(pdflib::PDFlib &p, int & tbl, int & row, int & col, int colmax, int font, int titlefont, const CVPara &p2, int fsize=10, int titlesize=12)
	{

		//int fsize=10;
		int quartercolmax=colmax/4;
		std::wostringstream optlist;
		CString str;

		optlist.str(L"");
		optlist << L"fittextline={position={left center} font=" << titlefont		
			<< L" fontsize=" << titlesize << L"}"
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

	};


	static void AddSAP(pdflib::PDFlib &p, int & tbl, int & row, int & col, int colmax, int font, int titlefont, const SAPara &p3, int fsize=10, int titlesize=12)
	{

		int seventhcolmax=(colmax/7)+1;
		int firstcol=colmax-6*seventhcolmax;

		std::wostringstream optlist;
		CString str;


		optlist.str(L"");
		optlist << L"fittextline={position={left center} font=" << titlefont		
			<< L" fontsize=" << titlesize << L"}"
			<< L" colspan=" << colmax
			;



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
	};


	static void AddRES(pdflib::PDFlib &p, int & tbl, int & row, int & col, int colmax, int font, int titlefont, const std::vector<CString> &res, int fsize=10, int titlesize=12)
	{

		//int fsize=10;
		int quartercolmax=colmax/4;

		//int seventhcolmax=(colmax/7)+1;
		//int firstcol=colmax-6*seventhcolmax;

		std::wostringstream optlist;
		CString str;

		optlist.str(L"");
		optlist << L"fittextline={position={left center} font=" << titlefont		
			<< L" fontsize=" << titlesize << L"}"
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

	};


	static int pdfout6(
		pdflib::PDFlib &p, 
		const ANPara &para, 
		const std::vector<CString> &res, 
		const CVPara &p2, 
		const SAPara &p3,
		bool b1,
		bool b2,
		bool b3,
		bool b4,
		const std::wstring logofile
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

			int titlefont;


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
			titlefont = p.load_font(L"Times-Bold", L"winansi", L"");
			if (titlefont == -1) {
				std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
				return(2);
			}

			optlist.str(L"");
			optlist << L"fittextline={position=center font=" << titlefont
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

			AddGI(p,tbl,row,col,colmax,font,titlefont,fsize);



			/* ---------- Row 1: table header (spans all columns) */


			if(b1){

				optlist.str(L"");
				optlist << L"fittextline={position={left center} font=" << font		
					<< L" fontsize=" << fsize << L"}"
					<< L" colspan=" << colmax
					;
				tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());

				row++;

				AddANP(p,tbl,row,col,colmax,font,titlefont,para,fsize);

			}


			/* ---------- Row 1: table header (spans all columns) */

			if(b2){
				optlist.str(L"");
				optlist << L"fittextline={position={left center} font=" << font		
					<< L" fontsize=" << fsize << L"}"
					<< L" colspan=" << colmax
					;
				tbl = p.add_table_cell(tbl, col, row, L" ", optlist.str());

				row++;

				AddCVP(p,tbl,row,col,colmax,font,titlefont,p2,fsize);
			}





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

				AddSAP(p,tbl,row,col,colmax,font,titlefont,p3,fsize);
			}

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


				AddRES(p,tbl,row,col,colmax,font,titlefont,res,fsize);
			}








			optlist.str(L"");
			if(b3){
				optlist <<L"fill={";
				for(int i=sarowend-p3.saplist.size()-1;i<sarowend;i+=2){
					optlist << L"{area=row"
						<< i
						<< L" fillcolor={gray 0.9}} ";
				}
				optlist << L"} "; 

			}
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
	};


	static int imgout2(
		pdflib::PDFlib &p, 
		//CanalyzerDoc *padoc, 
		CDC *pdc,
		std::vector<PlotDataEx> &imagelist, 
		std::vector<CString> &namelist, 
		CSize szimg,
		const std::wstring logofile)
	{

		if(imagelist.empty() || namelist.empty())
			return 3;

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



		font = p.load_font(L"Times-Bold", L"winansi", L"");
		//font = p.load_font(L"Helvetica", L"winansi", L"");
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

				//SaveImage(imagelist.back(), szimg, fptmp, pdc);
				imagelist.back().SaveImage(fptmp,szimg,pdc);

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




	};







public:
	DocDataEx ddex;
	std::vector<PlotDataEx> va;
	std::vector<PlotDataEx> tc;

public:
	PDFPrinter(){};
	~PDFPrinter(){};

	int pdfd(CString outfile, 
		bool b1,
		bool b2,
		bool b3,
		bool b4,
		bool b5,
		bool b6,
		bool b7
		)
	{

		const std::wstring searchpath = L"../data";

		const std::wstring temppdf = L"temp.pdf";

		pdflib::PDFlib p;

		std::wostringstream optlist;

		optlist.str(L"");

		p.set_parameter(L"errorpolicy", L"return");

		p.set_parameter(L"SearchPath", searchpath);

		if (p.begin_document(temppdf, optlist.str()) == -1) {
			//if (p.begin_document((LPCWSTR)outfile, L"") == -1) {
			std::wcerr << L"Error: " << p.get_errmsg() << std::endl;
			return 2;
		}
		p.set_info(L"Creator", L"PDFlib starter sample");
		p.set_info(L"Title", L"starter_table");


		CString templogobmp=L"templogo.bmp";



		CBitmap bmp;
		bmp.LoadBitmap(IDB_BITMAP_SINYANG);
		CImage img;
		//img.LoadFromResource(NULL,IDB_BITMAP_SINYANG);
		img.Attach(HBITMAP(bmp));
		HRESULT hResult = img.Save((LPCTSTR)templogobmp);
		bmp.DeleteObject();



		int a;



		std::vector<CString> res;
		if(b2){
			bool flg=ddex.GetResultString(res);
		}


		a=pdfout6(p,ddex.p1,res,ddex.p2,ddex.p3,b1,b2,b3,b4,templogobmp.GetBuffer());

		if(b5){
			a=pdfout(p,ddex.dol,templogobmp.GetBuffer());
		}


		std::vector<PlotDataEx> pdl;
		std::vector<CString> nl;

		if(b7){
			pdl.assign(va.begin(),va.end());
			CString str;
			str.LoadStringW(IDS_STRING_VOLTAMMOGRAM);
			nl.assign(va.size(),str);
			if(b6){
				pdl.resize(tc.size()+pdl.size());
				std::copy_backward(tc.begin(),tc.end(),pdl.end());
				str.LoadStringW(IDS_STRING_TEST_CURVE);
				nl.resize(tc.size()+nl.size(),str);
			}
		}
		else{
			if(b6){
				pdl.assign(tc.begin(),tc.end());
				CString str;
				str.LoadStringW(IDS_STRING_TEST_CURVE);
				nl.assign(tc.size(),str);
			}
		}

		for(size_t i=0;i<pdl.size();i++){
			pdl[i].ResetRange();
			pdl[i].pd.ps.SetStandradCr();
		}

		//CDC dc;
		//dc.CreateCompatibleDC(NULL);

		a=imgout2(p,
			//&dc,
			::AfxGetMainWnd()->GetDC(),
			pdl,nl,CSize(1000,690),templogobmp.GetBuffer());

		p.end_document(optlist.str());

		AddPageNumber(temppdf,(LPCWSTR)outfile);

		CFile::Remove(temppdf.c_str());
		CFile::Remove(templogobmp);

		return 0;
	};


	void work()
	{

		//TCHAR szFilters[]= _T("PDF Files (*.pdf)|*.pdf||");
		CString szFilters;
		szFilters.LoadStringW(IDS_FILTER_PDF);
		szFilters+=L"|";

		CFileDialog se(FALSE,L"pdf",TimeString()+L".pdf",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilters);

		CString str;

		BOOL chk1=TRUE,chk2=TRUE,chk3=TRUE,chk4=TRUE,chk5=TRUE,chk6=TRUE,chk7=TRUE;

		str.LoadStringW(IDS_STRING_ANALYSIS_PARA);
		se.AddCheckButton(IDS_STRING_ANALYSIS_PARA,str,chk1);

		str.LoadStringW(IDS_STRING_CV_PARA);
		se.AddCheckButton(IDS_STRING_CV_PARA,str,chk2);

		str.LoadStringW(IDS_STRING_ADDITION_SOLUTION_PARA);
		se.AddCheckButton(IDS_STRING_ADDITION_SOLUTION_PARA,str,chk3);

		str.LoadStringW(IDS_STRING_RESULT);
		se.AddCheckButton(IDS_STRING_RESULT,str,chk4);

		str.LoadStringW(IDS_OUTPUT_WND);
		se.AddCheckButton(IDS_OUTPUT_WND,str,chk5);

		str.LoadStringW(IDS_STRING_TEST_CURVE);
		se.AddCheckButton(IDS_STRING_TEST_CURVE,str,chk6);

		str.LoadStringW(IDS_STRING_VOLTAMMOGRAM);
		se.AddCheckButton(IDS_STRING_VOLTAMMOGRAM,str,chk7);


		if(se.DoModal()==IDOK){

			CString fp = se.GetPathName();

			
			se.GetCheckButtonState(IDS_STRING_ANALYSIS_PARA,chk1);
			se.GetCheckButtonState(IDS_STRING_CV_PARA,chk2);
			se.GetCheckButtonState(IDS_STRING_ADDITION_SOLUTION_PARA,chk3);
			se.GetCheckButtonState(IDS_STRING_RESULT,chk4);
			se.GetCheckButtonState(IDS_OUTPUT_WND,chk5);
			se.GetCheckButtonState(IDS_STRING_TEST_CURVE,chk6);
			se.GetCheckButtonState(IDS_STRING_VOLTAMMOGRAM,chk7);

			str.LoadString(IDS_STRING_REPORTING);
			str+=fp;

			if(pdfd(fp,chk1,chk2,chk3,chk4,chk5,chk6,chk7)==0){

				str.LoadStringW(IDS_STRING_REPORT);
				CString tmps;
				tmps.LoadStringW(IDS_STRING_IS_SAVED);

				//mf->GetCaptionBar()->ShowMessage(str+L" "+se.GetFileName()+L" "+tmps);
				ShellExecute(NULL, L"open", fp, NULL, NULL, SW_SHOW);			
			}
			else{
				AfxMessageBox(IDS_STRING_SAVE_ERROR);

				//str.LoadString(IDS_STRING_SAVE_ERROR);
				//mf->GetCaptionBar()->ShowMessage(str);
			}

		}
	};

};
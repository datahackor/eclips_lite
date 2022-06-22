// dpymgr.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "dpymgr.h"
#include <public.h>
#include <libsys.h>
#include <libutil.h>
#include <libwindux.h>
#include <signal_private.h>
#include <libcsutil.h>
#include <cls2struct.h>

#pragma   comment(lib,"libsys.lib") 
#pragma   comment(lib,"libutil.lib") 
#pragma   comment(lib,"libcsutil.lib") 
#pragma   comment(lib,"libwindux.lib")
#pragma   comment(lib,"ws2_32.lib") 


struct window_info_t
{
	void* hwnd;
	int pid;
	int field_8;
	int depthortime;
	int status;
	int window_no;
	int X;
	int Y;
	int width;
	int height;
	int Iconic;
	int index;
	char window_title[256];
	char dpy_host_name[80];
};

// Global Variables:
struct symbol_node_t {
	symtab_t* first;
	symtab_t* curr;
}; 

/*
* 引用汇编程序中的变量
* 就是在ASM代码中定义的变量，不是在本地定义的
* 汇编代码声明用 public C xxx
*/

extern "C" symtab_t pdfsym_root[];
extern "C" symbol_node_t pdfsym_list;
extern "C" symtab_t symtab_next;
extern "C" symtab_t* p_symtab_curr;
extern "C" int s_level;
extern "C" int symtab_inited;
extern "C" symtab_t* symtab[];
extern "C" symtab_node_t p_sym_node;
extern "C" char str_crt[8];
/*
* 
* 给汇编代码使用的变量，变量定义在本地，函数在 ASM 代码中
* 汇编代码用 EXTERN xxx:type
*/

extern "C" DWORD yyin = 0; // FILE*
extern "C" DWORD yyout = 0; // FILE*
extern "C" int yyparse();
extern "C" FILE * __cdecl __iob_func(void) {
	static FILE iob[] = { *stdin, *stdout, *stderr };
	return iob;
}


//symtab_t pdfsym_root[] =
//{ 
//	{ (char*)"Level0", 0, 0, 0, &pdfsym_root[0],0, &pdfsym_root[1] },
//	{ (char*)"Level1", 1, 1, 0, &pdfsym_root[1], &pdfsym_root[0],0 }
//};

//symbol_node_t pdfsym_list = { &pdfsym_root[0] ,&pdfsym_root[1] };
//symtab_t symtab_next = { 0, 0, 0, 0, &symtab_next, 0,0 };
//
//symtab_t * p_symtab_curr = &pdfsym_root[1];
//int s_level = 1;
//int symtab_inited = 1;


/*
* 20 种符号 
* 最多 20个 Section，也就是最多20个进程
*/
//symtab_t* symtab[20] = { &pdfsym_root[0] };


char ClassName[] = "dpymgr";
char WindowName[2] = "";
HINSTANCE hInst;                                // current instance
int ei_no = 0;
HWND hWnd = 0;
char procName[256], str_dpymgr_hwnd[24], octname[24];
char* pCmdLine = 0;
int dpy_shm_id = 0;
int msgqid_dpymgr;
int not_found = 0;
FILE* fp;
window_info_t winds[100];
int window_no, pid;
char* p_bin_dir = 0;
int shm_buff_size = 0;
char EnvString[300];
ei_info_t* p_eiinfo = 0;
int heart_breaks;
int process_tag = 0;
int p_dpy_frame_ptr = 0; // 共享内存数据帧指针偏移
int p_dpy_shm = 0; // 共享内存起始地址
int window_index = 0;
MSG_P msgp;

char included_filepath[260];
char include_path[512];
char include_file[260];
char pdfsym_file[600], pdf_cppout_file[600],pdf_yyout_file[100];
char cmdline[4096];
char ipcfile[260];
int pdfsym_filesize = 0;
char *p_pdfsymbuf = 0 ;
/*
* libmeta
*/
int libmeta_out = 0;

char* other_acc = 0;


int sym_succeessed = 0;
char not_first_time = 0;
int last_frame_ptr = 0;
data_type_tbl_t* data_type_tbl = 0;
char msgBuffer[800];
char* p_octname = 0;
char* p_custom_pdf_file = 0;
char* p_custom_pdf_lockfile = 0;

char first_pdf_open = 1;
char nullstring[2] = { 0 };
int token_num = -1;
char* token_array[10];
char* p_include_path = 0;
char* p_d_path = 0;
char pdf_path[26000];

char* p_pdf_path0 = 0;
char* p_pdf_include_paths = 0;
char* p_pdf_path2 = 0;
char* p_pdf_path_next = 0;
char* p_pdf_incl_path_text = 0;
int start_win_no = 0;
int last_block_id = 0;
char str_outbuf[300];
byte proc_normal = 1;


void init_dpy_start_acq();
void init_sym_table();
const char* __cdecl DuN_GetToken(const char* text);


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


void __cdecl quit(int code)
{
	int i = 0;
	if ((int)window_no > 0)
	{
		do
		{
			if (winds[i].pid > 0)
				kill(winds[i].pid, 9);
			++i;
		} while (i < window_no);
	}
	if (shmctl(dpy_shm_id, 0, 0) == -1)
		cls2_printf("%s: dpy_shm_id %d not removed\n", procName, dpy_shm_id);
	msgctl(msgqid_dpymgr, 0, 0);
	return windux_exit(code);
}

void __cdecl sigint_handler(int, int)
{
	return quit(0);
}
int __cdecl set_sig_break_handler()
{
	sigvec vec;
	vec.sv_handler = sigint_handler;
	vec.sv_flags = 0;
	vec.sv_mask = 0;

	if (sigvector(SIGINT, &vec, 0) != -1)
		return 0;
	cls2_printf("%s: sigvector SIGINT failed\n", procName);
	return -1;
}
int __cdecl CompareFunction(window_info_t* w1, window_info_t* w2)
{
	if (w1->index <= w2->index)
		return (w1->index >= w2->index) - 1;
	else
		return 1;
}

/*
* Geometry 267x223+42+243
* 格式为 ：
* width x height + X + Y
*/
int __cdecl save_dpy_file(char* proc_name, window_info_t* p_winds, int num, const char* oct_name)
{
	FILE* f;

	const char* p_iconic;
	char dpyfileName[260];

	qsort(p_winds, num, 0xE4u, (_CoreCrtNonSecureSearchSortCompareFunction)CompareFunction);
	sprintf(dpyfileName, "%s.dpy", oct_name);
	f = fopen(dpyfileName, "wb");

	if (!f)
		return cls2_printf("%s: save_dpy_file: Open failed for %s\n", proc_name, dpyfileName);
	fprintf(f, "OCTInitLaunch YES;\n");
	if (num > 0)
	{

		int L = num;
		do
		{
			p_iconic = "FALSE";
			if (p_winds->pid && p_winds->status != -1 && strlen(p_winds->window_title))
			{
				if (p_winds->Iconic)
					p_iconic = "TRUE";
				if (p_winds->X < 0)
					p_winds->X = 0;
				if (p_winds->Y < 0)
					p_winds->Y = 0;
				if (p_winds->width > 0 && p_winds->height > 0)
				{
					fprintf(f, "Iconic %s;\n", p_iconic);
					fprintf(f, "Geometry %ldx%ld+%ld+%ld;\n", p_winds->width, p_winds->height, p_winds->X, p_winds->Y);
				}
				if (strlen(p_winds->dpy_host_name))
					fprintf(f, "DisplayName \"%s\";\n", p_winds->dpy_host_name);
				fprintf(f, "IncludedFileName \"%s\";\n", p_winds->window_title);
				if (strstr(p_winds->window_title, ".log"))
					fprintf(f, "#undef DEPTHGRID\n");
				fprintf(f, "#include <%s>\n", p_winds->window_title);
			}
			p_winds++;
			--L;
		} while (L);
	}
	fclose(f);
	return cls2_printf("%s: save_dpy_file: Display status saved to %s\n", proc_name, dpyfileName);
}

void __cdecl exit_proc(int, int)
{
	shmctl(dpy_shm_id, 0, 0);
	msgctl(msgqid_dpymgr, 0, 0);
	if (!not_found)
		save_dpy_file(procName, (window_info_t*)winds, window_no, octname);
	if (fp)
		fclose(fp);
	return windux_exit(0);
}

int __cdecl set_sig_usr1_handler()
{
	sigvec vec;
	vec.sv_handler = exit_proc;
	vec.sv_flags = 0;
	vec.sv_mask = 0;


	if (sigvector(SIGUSR1, &vec, 0) != -1)
		return 0;

	cls2_printf("%s: sigaction SIGUSR1 failed", procName);
	return -1;
}

void init_eiinfo()
{

	p_eiinfo = (ei_info_t*)(p_dpy_shm + 0x13C);
	*(_DWORD*)(p_dpy_shm + 0x13C) = ei_no;
	char*p = getenv("EIPOST_FILE_NAME");
	strcpy(p_eiinfo->eipost_filename, p);
	p = getenv("IPCFILE");
	strcpy(p_eiinfo->ipcfile, p);

	p = getenv("EI_CONTROL_HOST");
	strcpy(p_eiinfo->ei_control_host, p);

	p = getenv("ACQ_BIN_DIR");
	strcpy(p_eiinfo->acq_bin_dir, p);


	return ;
}

int init_dpybufmgr()
{
	struct _stat32 fs_; 
	const char* argv[10];
	char name[40];
	char strBufferSize[16];

	data_type_tbl = (data_type_tbl_t*)get_data_type_tbl();
	shm_buff_size = 12000000;
	start_win_no = 1;
	window_no = 1;
	p_dpy_shm = create_priv_shm(&dpy_shm_id, 12000000);
	if (p_dpy_shm)
	{
		sprintf(EnvString, "DPY_SHM_ID=%d", dpy_shm_id);
		if (putenv(EnvString))
		{
			cls2_printf("%s: Could not add DPY_SHM_ID to environment\n", procName);
			return -1;
		}
		else
		{
			init_dpy_start_acq();
			char*p = getenv("IPCFILE");
			strcpy(ipcfile, p);

			p_bin_dir = getenv("ACQ_BIN_DIR");
			p = getenv("SYS_WORK_DIR");
			if (!p || _stat32(p, &fs_))
			{
				sprintf(
					cmdline,
					"%s %s \"%s/dpysenderr\" %d \"%s\" \"'%s'\" &",
					"remsh",
					pCmdLine,
					p_bin_dir,
					ei_no,
					ipcfile,
					"Working directory not accessible to display subsystem\nSee Message Window for Errors");
				// windux_system(cmdline);
				gethostname(name, 40);
				p = getenv("SYS_WORK_DIR");
				cls2_printf("%s: The system working directory, %s on %s, is not \n", procName, p, pCmdLine);
				cls2_printf(
					"    accessible from %s.  It must be mounted for display\n"
					"    programs to use files in it for repeat or correlate\n",
					name);
			}
			init_eiinfo();
			sprintf(strBufferSize, "%d", shm_buff_size);
			argv[0] = "dpybufmgr";
			argv[1] = str_dpymgr_hwnd;
			argv[2] = "WINDERZ";
			argv[3] = "99";
			argv[4] = pCmdLine;
			argv[5] = strBufferSize;
			argv[6] = 0;
			pid = _setjmp(windux_fork_jump_buf);
			windux_fork();
			if (!pid)
			{
				windux_execvp("dpybufmgr", argv);
				sprintf(msgBuffer, "%s: exec of dpybufmgr failed", procName);
				cls2_perror(msgBuffer);
				quit(1);
			}
			return 0;
		}
	}
	else
	{
		sprintf(msgBuffer, "%s: could not create DPY shared memory", procName);
		cls2_perror(msgBuffer);
		return -1;
	}
}


int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG Msg;
	char* p_ei = getenv("EIN");
	ei_no = atoi(p_ei);
	sprintf(procName, "%s[%d]", "dpymgr", ei_no);

	// Initialize global strings

	MyRegisterClass(hInstance);

	HWND Window = CreateWindowEx(0, ClassName, WindowName, 0xCF0000u, 0x80000000, 0, 0x80000000, 0, 0, 0, hInstance, 0);
	if (!Window)
	{
		int LastError = GetLastError();
		FormatMessage(0x1300u, 0, LastError, 0x400u, (LPSTR)&hInstance, 0, 0);
		cls2_printf("%s: CreateWindow failed\n%s\n", procName, (const char*)hInstance);
		windux_exit(-1);
	}

	char curr_path[1024] = { 0 };
	getcwd(curr_path, 1024);
	chdir(curr_path);

	hWnd = Window;
	sprintf(str_dpymgr_hwnd, "%d", (int)Window);
	set_exec_mode(0x8000000);
	pCmdLine = lpCmdLine;
	sigsetmask(0);
	init_winsock();
	register_resource(1, getpid());
	if (set_sig_break_handler() == -1)
		quit(-1);
	if (set_sig_usr1_handler() == -1)
		quit(-1);

	/*

	FILE* ff = fopen("pdfsym.txt", "rb");
	if (ff == NULL)
	{
		cls2_printf("open 'pdfsym.txt' fail errno = %d reason = %s \n", errno, strerror(errno));
	}

	fseek(ff, 0, SEEK_END);
	pdfsym_filesize = ftell(ff);
	p_pdfsymbuf = (char*)malloc(pdfsym_filesize + 1);
	fseek(ff, 0, SEEK_SET);
	fread(p_pdfsymbuf, pdfsym_filesize, 1, ff);
	fclose(ff);
	
	*/

	if (init_dpybufmgr() == -1)
	{
		cls2_printf("%s: initialization failed\n", procName);
		quit(-1);
	}
	while (GetMessage(&Msg, 0, 0, 0))
		DispatchMessage(&Msg);
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	memset(&wcex.hIcon, 0, 12);
	wcex.lpszMenuName = "";
	wcex.lpszClassName = "dpymgr";
	wcex.hIconSm = 0;

	return RegisterClassEx(&wcex);
}


void __cdecl convert_dpy_init_hdr(DPY_INIT_HDR* phdr)
{

	int cdss_num;

	swap4((byte*)phdr, 5);
	DPY_CDSS_INFO* p_cdss_info = (DPY_CDSS_INFO*)((char*)phdr + phdr->struct_size);
	if (phdr->cdss_num > 0)
	{

		cdss_num = phdr->cdss_num;
		do
		{
			swap4((byte*)p_cdss_info, 6);
			int L = p_cdss_info->num_curves;

			DPY_CURVE_INFO* p_crv_block = (DPY_CURVE_INFO*)((char*)phdr + p_cdss_info->curve_offset);

			if (p_cdss_info->num_curves > 0)
			{
				do
				{
					swap2((byte*)&p_crv_block->num_tools, 1);
					swap4((byte*)&p_crv_block->tool_id_offset, 2);
					swap2((byte*)&p_crv_block->cls_data_type, 3);
					swap4((byte*)p_crv_block->rel_dep_tim, 3);
					p_crv_block++;
					--L;
				} while (L);
			}
			++p_cdss_info;
			--cdss_num;
		} while (cdss_num);
	}
	return;
}

void init_dpy_start_acq()
{
	DPY_START_ACQ* p_frame_hdr = (DPY_START_ACQ*)p_dpy_shm;          // DPY_START_ACQ
	DPY_FRAME_INFO* p_frame_info = (DPY_FRAME_INFO*)(p_dpy_shm + 0x128);
	p_frame_info->frame_hdr_size = 0x144;
	p_frame_hdr->workmode = 0;
	p_frame_hdr->log_dir_record = 1;
	p_frame_hdr->drive_mode = 0;
	p_frame_info->frame_no = 1;

	last_frame_ptr = 0;
	return;
}


int wait_for_response(int window_no)
{

	int result;
	MSG Msg;
	heart_breaks = 0;
	if (winds[window_no].status != 0)
	{
		do
		{
			if (winds[window_no].status == -1)
				break;
			GetMessage(&Msg, 0, 0, 0);
			result = DispatchMessage(&Msg);
			if (heart_breaks > 2)
			{
				result = cls2_printf("%s: Window %d [%s] QUIT responding\n", procName, window_no, winds[window_no].window_title);
				winds[window_no].status = -1;
			}
		} while (winds[window_no].status);
	}
	return result;
}


void convert_dpy_curve_data()
{

	frame_t* p_frame;
	DPY_CDSS_INFO* p_dpy_cdss_info;
	frame_head_t* p_frame_head;
	frame_data_t* p_frame_data;
	int num_curves;

	int hdr_size = *(_DWORD*)(p_dpy_shm + 0x128);
	int cdss_of = *(_DWORD*)(hdr_size + p_dpy_shm + 12);
	int pbuf = p_dpy_shm + hdr_size;
	p_frame = (frame_t*)(p_dpy_shm + *(_DWORD*)(p_dpy_shm + 0x12C));
	p_dpy_cdss_info = (DPY_CDSS_INFO*)(pbuf + cdss_of);

	swap4((byte*)p_frame, 8);

	p_frame_head = (frame_head_t*)((char*)p_frame + p_frame->struct_size);

	if (p_frame->frame_num > 0)
	{
		int frame_num = p_frame->frame_num;
		do
		{
			swap4((byte*)p_frame_head, 6);
			p_frame_data = (frame_data_t*)((char*)p_frame_head + p_frame_head->frame_data_offset);
			if (p_frame_head->count > 0)
			{
				int L = p_frame_head->count;
				do
				{
					swap4((byte*)p_frame_data, 6);
					num_curves = p_dpy_cdss_info[p_frame_data->cdss_no].num_curves;
					DPY_CURVE_INFO* p_crv_block = (DPY_CURVE_INFO*)(pbuf + p_dpy_cdss_info[p_frame_data->cdss_no].curve_offset);
					if (num_curves > 0)
					{
						int* p_curve_data_buf = (int*)((char*)p_frame_data + p_frame_data->curve_array_offset);

						do
						{
							swap4((byte*)p_curve_data_buf, 1);
							if (*p_curve_data_buf)
							{
								void* p_data = (char*)p_frame_data + *p_curve_data_buf;
								int siz = data_type_tbl[p_crv_block->cls_data_type].size;
								if (siz == 4)
								{
									swap4((byte*)p_data, p_crv_block->num_ele);
								}
								else if (siz == 2)
								{
									swap2((byte*)p_data, p_crv_block->num_ele);
								}
							}
							++p_curve_data_buf;
							++p_crv_block;
							--num_curves;
						} while (num_curves);

					}
					p_frame_data = (frame_data_t*)((char*)p_frame_data + p_frame_data->data_offet);
					--L;
				} while (L);
			}
			p_frame_head = (frame_head_t*)((char*)p_frame_head + p_frame_head->frame_size);

		} while (frame_num-- != 1);
	}
	return;
}
void __cdecl msg_dpy_curve_data(OUTBUF_HDR* p_hdr)
{

	float rate;
	/*
	* 指向数据的偏移
	*/
	*(_DWORD*)(p_dpy_shm + 0x12C) = p_hdr->data_offset + 8;

	convert_dpy_curve_data();

	rate = (float)(*(_DWORD*)(p_dpy_shm + 0x138) - last_block_id) * 0.03125f;
	if (rate > 1.0)
		rate = 1.0;
	if (*(_DWORD*)(p_dpy_shm + 0x130) && rate < 0.5)
	{
		*(_DWORD*)(p_dpy_shm + 0x130) = 0;
		cls2_printf("%s: Display buffer usage normal\n", procName);

	}
	int i = start_win_no;
	if (start_win_no < window_no)
	{
		do
		{
			if (winds[i].status == 0)
			{

				int lvl = 4 * (int)(((float)winds[i].field_8 * rate));

				if (p_hdr->block_id - winds[i].depthortime > lvl)
				{
					winds[i].depthortime = p_hdr->block_id;
					winds[i].status = 2;
					PostMessage((HWND)winds[i].hwnd, 0x403u, *(_DWORD*)(p_dpy_shm + 0x12C), 0);
					wait_for_response(i);

				}
			}
			++i;

		} while (i < window_no);
	}
}

void __cdecl convert_aif_frame(aif_frame_hdr_t* p_frame)
{
	swap4((byte*)p_frame, 8);
	swap4((byte*)&p_frame->file_hdr, 1);
	swap4((byte*)&p_frame->file_hdr.file_state, 10);
	swap4((byte*)&p_frame->file_hdr.file_type, 2);
	swap4((byte*)&p_frame->file_hdr.dsc_status, 1);

	for (int i = 0; i < 49; i++)
	{
		swap4((byte*)&p_frame->file_hdr.aif_rec_type_info[i], 3);
	}
	return;
}

/*
* cdss_num == num_frames
*/
void __cdecl convert_aif_crv(aif_frame_t* p_frame, int num_frames)
{

	int struct_size = 0x14;
	int tool_id_struct_size = 12;


	for (int i = 0; i < num_frames; i++)
	{
		swap4((byte*)p_frame, 8);
		
		aif_frame_info_t* p_frame_info = &p_frame->frame_info;
		swap4((byte*)p_frame_info, 5);

		int num_curves = p_frame->frame_info.num_curves;

		aif_crv_info_t* p_crv_info = (aif_crv_info_t*)((char*)p_frame_info + struct_size);
		p_frame = (aif_frame_t*)p_crv_info;
		for (int j = 0; j < num_curves; j++)
		{
			
			swap4((byte*)&p_crv_info->struct_size, 1);
			swap4((byte*)&p_crv_info->aif_curve_name.struct_size, 2);
			swap4((byte*)&p_crv_info->cls_data_type, 1);
			swap2((byte*)&p_crv_info->curve_id, 2);
			swap4((byte*)&p_crv_info->rel_dep_tim, 6);

			int num_tools = p_crv_info->aif_curve_name.num_tools;

			p_crv_info++; // == (aif_crv_info_t* )((char*)p_crv_info + 0x78);
			aif_tool_id_t* p_aif_toolid = (aif_tool_id_t*)(p_crv_info);


			for (int k = 0; k < num_tools; k++)
			{
				swap4((byte*)p_aif_toolid, 1);
				p_aif_toolid++;
			}

			p_frame = (aif_frame_t*)(p_aif_toolid);
			p_crv_info = (aif_crv_info_t*)p_frame;
		}

	}

	return;
}



void __cdecl refresh_thread()
{
	while (1)
	{
		PostMessage(hWnd, 0x43Cu, 0, 0);
		Sleep(0x3E8u);
	}
}

int Du_GetCustomPdfPath()
{
	char* p_home; 
	char Buffer[2048]; 

	int result_error = 0;
	if (p_custom_pdf_file)
		return 0;
	if (getenv("EXPENV_SET"))
	{
		p_home = getenv("HOME");
		if (!p_home)
		{
			fprintf(stderr, "Du_GetPdfPath: Error HOME variable is not set\n");
			windux_exit(8);
		}
	}
	else
	{
		p_home = getenv("ACQ_DPY_DIR");
		if (!p_home)
		{
			fprintf(stderr, "Du_GetPdfPath: Error ACQ_DPY_DIR is not set\n");
			windux_exit(8);
		}
	}
	sprintf(Buffer, "%s\\.custom-pdf.%s", p_home, "txt");
	if (first_pdf_open)
	{
		p_custom_pdf_file = (char*)malloc(strlen(Buffer) + 1);
		strcpy(p_custom_pdf_file, Buffer);

		FILE *fp = fopen(p_custom_pdf_file, "r");
		if (fp)
			fclose(fp);
		else
			result_error = 1;
		p_custom_pdf_lockfile = (char*)malloc(strlen(p_custom_pdf_file) + 10);
		sprintf((char* const)p_custom_pdf_lockfile, "%s.lock", p_custom_pdf_file);
	}
	first_pdf_open = 0;
	FILE* fp = fopen(p_custom_pdf_file, "a");
	if (!fp)
	{
		fprintf(stderr, "Du_GetPdfPath: Improper access permissions for file: %s.\n", p_custom_pdf_file);
		windux_exit(8);
	}
	fclose(fp);
	return result_error;
}


/*
* 将path 连接到 root，如果内存不足则自动分配
*/
unsigned int concat_path(const char** p_root, const char* p_path)
{
	unsigned int length; 

	char* pp = (char*)*p_root;
	length = strlen(p_path);
	if (length)
	{
		char *pend = &pp[strlen(pp) + 1];
		pp = (char*)realloc((void*)*p_root, strlen(p_path) + pend - (*p_root + 1) + 7);
		*p_root = pp;
		if (!pp)
		{
			fprintf(stderr, "Fatal realloc(1) error in Du_GetPdfPath\n");
			windux_exit(8);
		}
		length = strlen(p_path) + 1;
		qmemcpy((void*)&(*p_root)[strlen(*p_root)], p_path, length);
	}
	return length;
}
void DuN_ParsePdf(char* d_path, const char** include_path)
{

	const char* p_token; 
	char lineBuffer[2048]; 
	char tokenbuf[2048];

	Du_GetCustomPdfPath();
	*d_path = 0;
	tokenbuf[0] = 0;
	FILE* fp = fopen(p_custom_pdf_file, "r");

	if (fp)
	{
		fgets(lineBuffer, 2047, fp);
		if (feof(fp))
		{
			 fclose(fp);
			 return;
		}
		else
		{
			fclose(fp);
			char *p = strchr(lineBuffer, '\r');
			if (p)
				*p = 0;
			p = strchr(lineBuffer, '\n');
			if (p)
				*p = 0;
			p = strstr(lineBuffer, " -D");
			if (p)
			{
				strcpy(d_path, p);
				*p = 0;
			}
			p = strstr(lineBuffer, "-D");
			if (p)
			{
				strcpy(d_path, p);
				*p = 0;
			}
			p_token = DuN_GetToken(lineBuffer);
			sprintf(tokenbuf, "%s ", p_token);
			concat_path(include_path, tokenbuf);
			return;
		}
	}
	return ;
}

const char* __cdecl DuN_GetToken(const char* text)
{

	if (token_num < 0)
	{
		token_array[0] = 0;
		token_array[1] = 0;
		token_array[2] = 0;
		token_array[3] = 0;
		token_array[4] = 0;
		token_array[5] = 0;
		token_array[6] = 0;
		token_array[7] = 0;
		token_array[8] = 0;
		token_array[9] = 0;
	}
	char*p = (char*)text;
	if (!text)
		return 0;
	/*
	* 忽略起始空白字符
	*/
	while (1)
	{
		char cc = *p;
		if (cc != ' ' && cc != '\t' && cc != '\n')
			break;
		++p;
	}
	int pos = strlen(p);
	if (!pos)
		return nullstring;
	int n_token = token_num + 1;
	token_num = n_token;
	if (n_token >= 10)
	{
		n_token = 0;
		token_num = 0;
	}
	if (token_array[n_token])
	{
		free(token_array[n_token]);
		token_array[n_token] = 0;
	}
	char *p_token = (char*)malloc(pos + 1);
	token_array[n_token] = p_token;
	strcpy(p_token, p);

	/*
	* 去掉尾部空白字符
	*/
	char *p_end = &p_token[pos - 1];
	do
	{
		char cc = *p_end;
		if (cc != ' ' && cc != '\t' && cc != '\n')
			break;
		--pos;
		--p_end;
	} while (pos);
	p_token[pos] = 0;
	return (const char*)token_array[n_token];
}

/*
* 将所有存在的包含路径读取处理
*/
char* Du_GetPdfPath()
{
	struct _stat32 fs_sta; 
	char dpy_default_dir[2048];
	char str_pdfincl[1968];
	const char* pdf_dir[2];
	char Buffer[2048];
	char FileName[2048];
	char iclud_path[2048]; 

	char* p_dpy_dir = getenv("ACQ_DPY_DIR");

	if (getenv("EXPENV_SET"))
	{
		char*p = (char*)malloc(1u);
		*p = 0;
		p_d_path = p;
		*p = 0;
		DuN_ParsePdf(iclud_path, (const char**)&p_d_path);
		char *p_dpy_pdfincl = getenv("ACQ_DPY_PDFINCL");
		if (!p_dpy_pdfincl)
			return p_dpy_pdfincl;

		strcpy(str_pdfincl, p_dpy_pdfincl);

		if (!p_dpy_dir)
			p_dpy_dir = (char*)nullstring;

		sprintf(dpy_default_dir, "-I\"%s\\%s\"", p_dpy_dir, "default");

		if (!strstr(str_pdfincl, dpy_default_dir))
		{
			sprintf(Buffer, "-I\"%s\"", getenv("ACQ_DPY_DIR"));
			char *ppath = strstr(str_pdfincl, Buffer);
			if (ppath)
			{
				char *pe = &ppath[&Buffer[strlen(Buffer) + 1] - &Buffer[1]];
				if (pe)
					*pe = 0;

				sprintf(FileName, "%s -I\"%s\\default\" %s", str_pdfincl, getenv("ACQ_DPY_DIR"), pe);
				strcpy(str_pdfincl, FileName);
			}
		}
		concat_path((const char**)&p_d_path, str_pdfincl);
		if (!p_dpy_dir)
			p_dpy_dir = (char*)nullstring;
		sprintf(dpy_default_dir, "-I\"%s\\%s\"", p_dpy_dir, "default");
		p = strstr(p_d_path, dpy_default_dir);
		if (p)
		{
			p[strlen(dpy_default_dir)] = 0;
			sprintf(Buffer, "%s\\%s", p_dpy_dir, "default");
			void *dir = opendir(Buffer);
			file_info_t*p_fileinfo = (file_info_t*)readdir(dir);
			if (p_fileinfo)
			{
				do
				{

					if (strcmp(p_fileinfo->d_file_name, "."))
					{
						if (strcmp(p_fileinfo->d_file_name, ".."))
						{
							sprintf(dpy_default_dir, "%s\\%s", Buffer, p_fileinfo->d_file_name);
							if (!_stat32(dpy_default_dir, &fs_sta) && (fs_sta.st_mode & 0x4000) != 0)
							{
								strcat(p_d_path, " -I");
								strcat(p_d_path, "\"");
								concat_path((const char**)&p_d_path, dpy_default_dir);
								strcat(p_d_path, "\"");
							}
						}
					}
					p_fileinfo = (file_info_t*)readdir(dir);
				} while (p_fileinfo);

			}
			closedir(dir);
			sprintf(dpy_default_dir, "-I\"%s\\%s\"", p_dpy_dir, "default");
			char *p_exist = strstr(str_pdfincl, dpy_default_dir);
			if (!p_exist)
			{
				free(p_d_path);
				return 0;
			}
			concat_path((const char**)&p_d_path, &p_exist[strlen(dpy_default_dir)]);
		}
		concat_path((const char**)&p_d_path, iclud_path);
		return p_d_path;
	}
	else
	{

		pdf_dir[0] = "local";
		pdf_dir[1] = "default";
		char* p = (char*)malloc(1u);
		*p = 0;
		p_include_path = p;
		*p = 0;
		char *p_pdf_workdir= getenv("PDF_SYS_WORK_DIR");
		if (p_pdf_workdir)
		{
			sprintf(Buffer, "-I\"%s\" ", p_pdf_workdir);
			concat_path((const char**)&p_include_path, Buffer);
		}

		DuN_ParsePdf(iclud_path, (const char**)&p_include_path);
		p_dpy_dir = getenv("ACQ_DPY_DIR");
		if (p_dpy_dir)
		{
			sprintf(Buffer, "-I\"%s\" ", p_dpy_dir);
			concat_path((const char**)&p_include_path, Buffer);
			int i = 0;

			do
			{
				sprintf(Buffer, "%s%c%s", p_dpy_dir, '\\', (const char*)pdf_dir[i]);
				void* dir = opendir(Buffer);
				file_info_t* p_fileinfo = (file_info_t*)readdir(dir);
				if (p_fileinfo)
				{
					do
					{
						if (strcmp(p_fileinfo->d_file_name, ".") )
						{
							if (strcmp(p_fileinfo->d_file_name, ".."))
							{
								sprintf(FileName, "%s%c%s", Buffer, '\\', p_fileinfo->d_file_name);
								if (!_stat32(FileName, &fs_sta) && (fs_sta.st_mode & 0x4000) != 0)
								{
									sprintf(FileName, " -I\"%s%c%s\"", Buffer, '\\', p_fileinfo->d_file_name);
									concat_path((const char**)&p_include_path, FileName);
								}
							}
						}
						p_fileinfo = (file_info_t*)readdir(dir);
					} while (p_fileinfo);



				}
				closedir(dir);
				++i;
			} while (i < 2);
			concat_path((const char**)&p_include_path, iclud_path);
		}
		return p_include_path;
	}
}


/*
* 处理包含路径，去除空格和包含标志，只保留纯路径文本
*/

int GetPdfPath()
{
	bool result;result = false;
	char*PdfPath = Du_GetPdfPath();
	p_pdf_path0 = PdfPath;
	if (!PdfPath)
	{
		fprintf(stderr, "Fatal error in Du_FindPdf(2)... Du_GetPdfPath = NULL\n");
		windux_exit(8);
		PdfPath = p_pdf_path0;
	}
	p_pdf_include_paths = PdfPath;
	int length = strlen(PdfPath);
	char *p = pdf_path;
	p_pdf_path_next = pdf_path;
	p_pdf_path2 = pdf_path;
	p_pdf_incl_path_text = pdf_path;
	int i = 0;
	if (length >= 0)
	{
		while (1)
		{
			char cc = *PdfPath;
			if (!*PdfPath)
			{
				p_pdf_path0 = pdf_path;
				return 0;
			}
			if (!result && cc == ' ')
			{
				*p = ';';
				p_pdf_path2 = ++p;
			}
			else
			{
				if (cc == '-' && PdfPath[1] == 'I')
				{
					++PdfPath;
					++length;
				}
				else
				{
					if (cc != '"')
					{
						*p = cc;
						p_pdf_path2 = ++p;
					}
					result = !result;
				}
			}

			++i;
			++PdfPath;
			if (i > length)
			{
				p_pdf_path0 = pdf_path;
				return 0;
			}
		}

	}

	p_pdf_path0 = pdf_path;
	return 0;
}

/*
* 给定包含的所有路径，查找文件名
*/
char* Du_ParseInclPath(const char* p_incl_filename)
{
	file_info_t* p_fileinfo; 

	char *p = p_pdf_incl_path_text;
	void* pdir = 0;
	p_pdf_path_next = p_pdf_incl_path_text;
	p_pdf_path2 = p_pdf_incl_path_text;
	p_pdf_path0 = p_pdf_incl_path_text;
	if (*p_pdf_incl_path_text == 0)
		return (char*)nullstring;
	while (1)
	{
		char CC = *p;

		char *p_dst = include_path;
		p_pdf_path2 = include_path;

		if (CC != 0)
		{
			/*
			* 分号断句，取出一条incude语句
			*/
			while (CC != ';')
			{
				++p;
				*p_dst = CC;
				CC = *p;
				++p_dst;
				if (CC == 0)
				{// 字符串结束,复制完毕
					break;
				}
			}
			if (*p == ';')
				p_pdf_path_next = p + 1; // 是分号，跳过分号
			else
				p_pdf_path_next = p;
			p_pdf_path2 = p_dst;
			/*
			* 根据取出的一条 include 语句 读取其包含的 内容
			* 其实就是包含文件的路径
			*/
			*p_dst = 0;
			void* pdir = opendir(include_path);

			if (pdir)
			{
				p_fileinfo = (file_info_t*)readdir(pdir);
				while (p_fileinfo)
				{

					if (*p_fileinfo->d_file_name != '.')
					{
						if (!strcmp(p_incl_filename, p_fileinfo->d_file_name))
						{
							closedir(pdir);
							sprintf(include_file, "%s%c%s", include_path, '\\', p_incl_filename);
							return include_file;
						}
					}
					p_fileinfo = (file_info_t*)readdir(pdir);

				}
				closedir(pdir);
			}

			p = p_pdf_path_next;
			if (!*p)
				return (char*)nullstring;
		}


	}


}

/*
* 根据文件名在include 路径中查找文件，如果存在则返回包含文件的全路径
*/
char* __cdecl Du_FindPdf(const char* p_incl_file)
{

	GetPdfPath();
	char *p = Du_ParseInclPath(p_incl_file);
	strcpy(included_filepath, p);

	free(p_pdf_include_paths);
	return included_filepath;
}
/*
* 存在返回 1
*/
char __cdecl check_pdf_file(char* pdfFileName)
{
	char String2[256];
	char String3[512];

	if (!pdfFileName || !strlen(pdfFileName))
		return 0;
	char *p = strrchr(pdfFileName, '/');
	if (p || (p = strrchr(pdfFileName, '\\')) != 0)
	{
		strcpy(String2, p + 1);
		strcpy(String3, pdfFileName);

		p = (char*)strrchr((const char*)String3, '/');
		if (!p)
			p = (char*)strrchr((const char*)String3, '\\');
		*p = 0;
	}
	else
	{
		String3[0] = '.';
		strcpy(String2, pdfFileName);
	}
	void*pdir = opendir(String3);
	if (!pdir)
		return 0;
	file_info_t *p_fileinfo = (file_info_t*)readdir(pdir);
	if (p_fileinfo)
	{
		while (stricmp(p_fileinfo->d_file_name, String2))
		{
			p_fileinfo = (file_info_t*)readdir(pdir);
			if (!p_fileinfo)
			{
				closedir(pdir);
				return 0;
			}
		}

	}

	closedir(pdir);
	return 1;
}

int __cdecl convert_to_dpy(char* procName, char* FileName, char* octname)
{

	char Buffer[260],String[200],Str[500]; 

	FILE*fp = fopen(FileName, "rb");
	if (!fp)
		return cls2_printf("%s: convert_to_dpy: Open failed for %s\n", procName, FileName);
	sprintf(Buffer, "%s.dpy", octname);
	FILE* fp1 = fopen(Buffer, "wb");
	if (!fp1)
		return cls2_printf("%s: convert_to_dpy: Open failed for %s\n", procName, Buffer);
	while (fgets(Str, 500, fp))
	{
		if (strstr(Str, "#include"))
		{
			int i = 0;
			strcpy(String, Str);
			strtok(String, "<\"");
			char *p = strtok(0, ">\"");
			if (p)
				fprintf(fp1, "IncludedFileName \"%s\";\n", p);
		}
		fprintf(fp1, "%s", Str);
	}
	strcpy(FileName, Buffer);
	fclose(fp);
	return fclose(fp1);
}

int __cdecl msgout(FILE* fp, const char* msg)
{
	int result = 0;

	if (!libmeta_out)
	{
		if (fp)
		{
			if (fp)
			{
				fprintf(fp, "%s", msg);
				return fflush(fp);
			}
			else
			{
				fprintf(fp, "%s", msg);
				return fflush(fp);
			}
		}
		else
		{
			libmeta_out = 1;
		}
	}
	return result;
}
void __cdecl msgexit(int Code)
{

	char Buffer[604]; 

	if (Code && !libmeta_out)
	{
		sprintf(Buffer, "Program exits with return code = %d\n", Code);

		msgout(stderr, Buffer);
	}
	exit(Code);
}



int __cdecl out_msg(const char* msg)
{

	sprintf(msgBuffer, "ERROR: %s (%d", msg, errno);

	msgout(stderr, msgBuffer);
	int e = errno;
	if (errno <= 0 || (e >= _sys_nerr))
	{
		sprintf(msgBuffer, ")\n");
		return msgout(stderr, msgBuffer);
	}
	else
	{
		char** elist = _sys_errlist;
		sprintf(msgBuffer, "; %s)\n", elist[errno]);

		return msgout(stderr,msgBuffer);
	}
}
/*
* 更新链表，指向最新位置
*/

int __cdecl add_to_table(struct symtab_t* tab)
{
	symtab[p_sym_node.s_level++]->s_curr = tab;
	symtab[p_sym_node.s_level] = tab;
	symtab[p_sym_node.s_level]->s_curr = tab;
	return p_sym_node.s_level;
}

int __cdecl file_read(int FileHandle, void* pbuf, unsigned int length)
{
	int nbytes; 
	unsigned int N;

	N = 0;
	while (N < length)
	{
		nbytes = read_(FileHandle, (char*)pbuf + N, length - N);
		if (nbytes == -1)
			return -1;
		N += nbytes;
		if (!nbytes)
			return N;
	}
	return N;
}

void* __cdecl my_alloc(size_t Count, size_t Size)
{
	void* p; 

	p = calloc(Count, Size);
	if (!p)
	{
		perror("\nPDF interface: Out of memory!\n");
		exit(1);
	}
	return p;
}


int __cdecl pdf_load_sym(char* fp_str, int typ)
{
	int FileHandle = -1; 
	symtab_t* p_tab; 
	size_t Size; 
	symtab_t* p_tab1;
	param_t* p_param;
	goto ignore_open_sym_file;
	if (1)
	{
		FileHandle = open_(pdfsym_file, _O_BINARY, _O_RDONLY);
		/*
		* 因为从文件加载，所以要跳过头部 level 0 和 level 1
		*/
		char hdr[80];
		if (file_read(FileHandle, hdr, 0x4Eu) != 0x4Eu)
		{
			out_msg("read");
			return 0;
		}

	}
	else
		FileHandle = atoi(fp_str);
ignore_open_sym_file:
	int first_node = 0;
	p_symtab_curr = &pdfsym_root[1];
	if (symtab_inited && FileHandle != -1)
	{
		p_tab = pdfsym_list.first;
		pdfsym_list.first->s_next = pdfsym_list.first;
		while (p_tab->s_next)
		{
			p_tab1 = p_tab;
			p_tab = (symtab_t*)my_alloc(1,0x1Cu);
			p_tab1->s_next = p_tab;
			if (file_read(FileHandle, p_tab, 0x1Cu) != 0x1C)
			{
				out_msg("read");
				return 0;
			}
			if (file_read(FileHandle, &Size, 4u) != 4)
			{
				out_msg("read");
				return 0;
			}
			p_tab->s_type = (char*)my_alloc(1,Size);
			int n = file_read(FileHandle, p_tab->s_type, Size);
			if (n != Size)
			{
				out_msg("read");
				return 0;
			}
			p_tab->s_prev = p_tab1;
			p_tab->s_curr = p_tab;

			if (p_tab->s_param)
			{
				p_param = (param_t*)my_alloc(1u, 0x10u);
				p_tab->s_param = p_param;
				if (file_read(FileHandle, p_param, 0x10u) != 16)
				{
					out_msg("read");
					return 0;
				}

				if (p_param->p_type == 259)
				{
					if (file_read(FileHandle, &Size, 4u) != 4)
					{
						out_msg("read");
						return 0;
					}
					char*p = (char*)my_alloc(1u, Size);
					p_param->p_value.p_str = p;
					int siz = file_read(FileHandle, p, Size);
					if (siz != Size)
					{
						out_msg("read");
						return 0;
					}

				}


				while (p_param->p_next)
				{
					param_t *p_old = p_param;
					p_param = (param_t*)my_alloc(1u, 0x10u);
					p_old->p_next = p_param;
					if (file_read(FileHandle, p_param, 0x10u) != 16)
					{
						out_msg("read");
						return 0;
					}
					if (p_param->p_type == 259)
					{
						if (file_read(FileHandle, &Size, 4u) != 4)
						{
							out_msg("read");
							return 0;
						}
						char*p = (char*)my_alloc(1u, Size);
						p_param->p_value.p_str = p;
						int siz = file_read(FileHandle, p, Size);
						if (siz != Size)
						{
							out_msg("read");
							return 0;
						}
					}
				}
			}
		}
		if (close_(FileHandle) == -1)
		{
			out_msg("close");
			return 0;
		}
		symtab_inited = 0;
	}
	p_sym_node.s_level = 0;
	add_to_table(pdfsym_list.first->s_next);
	if (typ == 1)
	{
		strcpy(p_sym_node.name, "crt?");
		strcpy(str_crt, "plt?");
	}
	else
	{
		if (typ != 2)
		{
			sprintf(msgBuffer, "\npdfapp: Invalid device type.\n");
			msgout(stderr, msgBuffer);
			exit(1);
		}
		strcpy(p_sym_node.name, "plt?");
		strcpy(str_crt, "crt?");
	}
	return 1;
}

/*
* pdf_cpp.exe -I. "p857d.dpy" "pdf_cppout.txt"
* 编译 p857d.dpy 
* 输出到 pdf_cppout.txt
*/

int __cdecl exec_pdf_cppout(char* p_file)
{

	const char* p_pdfcpp_exe; 
	char pdfcpp_exe[256]; 
	char envPathStr[268]; 
	const char* p_incl; 
	char cmdbuff[2048]; 

	p_incl = getenv("ACQ_DPY_PDFINCL");
	if (p_incl)
	{
		if (*p_incl != '-')
		{
			sprintf(cmdbuff, "pdfmgr: reset default ACQ_DPY_PDFINCL=-I.");

			msgout(stdout, cmdbuff);
			p_incl = "-I.";
		}
	}
	else
	{
		sprintf(cmdbuff, "pdfmgr: set default ACQ_DPY_PDFINCL=-I.");
		msgout(stdout, cmdbuff);
		p_incl = "-I.";
	}
	if (getenv("SYS_PDF_CPP"))
	{
		p_pdfcpp_exe = getenv("SYS_PDF_CPP");
		strcpy(cmdbuff, p_pdfcpp_exe);
	}
	else
	{
		strcpy(cmdbuff, "pp.exe");
	}
	strcat(cmdbuff, " ");
	if (getenv("SYS_PDF_CPP"))
	{
		p_pdfcpp_exe = getenv("SYS_PDF_CPP");
		strcpy(pdfcpp_exe, p_pdfcpp_exe);
	}
	else
	{
		strcpy(pdfcpp_exe, "pp.exe");
	}
	strtok(pdfcpp_exe, " ");
	_searchenv(pdfcpp_exe, "PATH", envPathStr);
	if (strlen(envPathStr))
	{
		/*
		* pp 
		*/
		if (other_acc)
			strcat(cmdbuff, other_acc);
	}
	else
	{
		strcpy(cmdbuff, "pdf_cpp.exe ");
		if (getenv("SYS_PDF_CPP"))
		{
			sprintf(envPathStr, "pdfmgr:Missing %s use pdf_cpp.exe instead.\n", getenv("SYS_PDF_CPP"));
		}
		else
		{
			sprintf(envPathStr, "pdfmgr:Missing %s use pdf_cpp.exe instead.\n", "pp.exe");
		}

		msgout(stdout, envPathStr);
	}
	strcat(cmdbuff, p_incl);
	strcat(cmdbuff, " ");
	strcat(cmdbuff, "\"");
	strcat(cmdbuff, p_file);
	strcat(cmdbuff, "\"");
	int length = strlen(cmdbuff);
	int len_cpp = strlen(pdf_cppout_file);
	char* p_cmdline = (char*)malloc(length + len_cpp + 256);
	if (p_cmdline)
	{
		sprintf(p_cmdline, "%s \"%s\"", cmdbuff, pdf_cppout_file);
		int ret = windux_system(p_cmdline) >> 8;
		if (ret == 33)
			ret = 0;
		if (ret <= 4)
		{
			if (ret > 0)
			{
				sprintf(cmdbuff, "Warnings encountered running: %s.\n", p_cmdline);
				cls2_printf(cmdbuff);
			}
			free(p_cmdline);
			if (other_acc)
			{
				free(other_acc);
				other_acc = 0;
			}
			yyin = (DWORD)fopen(pdf_cppout_file, "rb");
			if (yyin)
				return 0;
			else
				return -1;
		}
		else
		{
			sprintf(cmdbuff, "Fatal error(s) encountered running: %s.\n", p_cmdline);
			cls2_printf(cmdbuff);
			free(p_cmdline);
			return -1;
		}
	}
	else
	{
		sprintf(cmdbuff, "pdfmgr: malloc failure. Exit(8)");
		msgout(stdout, cmdbuff);
		return -1;
	}
}

/*
* 
	以如下section代码为例 ，将编译好的符号写给进程
	一般 section 包含2部分
	第1部分是程序本身的属性代码，
	第2部分是Section：包含程序的各种配置参数

	Iconic FALSE;
	Geometry 312x348 + 104 + 530;
	DisplayName "PROC_HOST";
	IncludedFileName "num.show";
	# 1 "D:\Projects\Reversing\eclips\Source\out\pdf\default\show/num.show" 1
	Section
	{
		 Program     show;
		 Arguments - title "Processed Curves"
					- display  PROC_HOST
					 - xrm "*iconName: Numeric"
					 - xrm "*iconic: FALSE"
					 - geometry + 5 - 20;
	  OctCurves   Num;
	}


	此函数可以写文件或者管道，
	注意这里使用 fwrite_ （VC8 Runtime）是为了与旧程序兼容
*/

int __cdecl write_sym_file(FILE* fp)
{
	int STOP; 
	symtab_t* p_sym;
	symtab_t* p_sym_1; 
	size_t ElementSize; 
	int zero; 

	zero = 0;
	STOP = 0;
	param_t* par;
	/*
	* 写第1部分
	*/
	for (p_sym = pdfsym_list.first; p_sym != symtab[p_sym_node.s_level]; p_sym = p_sym->s_next)
	{
		if (p_sym->s_level == 1)
		{
			if (fwrite_(p_sym, 0x1Cu, 1u, fp) != 1)
			{
				out_msg("fwrite");
				return -1;
			}
			ElementSize = strlen(p_sym->s_type) + 1;
			if (fwrite_(&ElementSize, 4u, 1u, fp) != 1)
			{
				out_msg("fwrite");
				return -1;
			}
			if (fwrite_(p_sym->s_type, ElementSize, 1u, fp) != 1)
			{
				out_msg("fwrite");
				return -1;
			}
			for (par = p_sym->s_param; par; par = par->p_next)
			{
				if (fwrite_(par, 0x10u, 1u, fp) != 1)
				{
					out_msg("fwrite");
					return -1;
				}
				if (par->p_type == 259)
				{
					ElementSize = strlen(par->p_value.p_str) + 1;
					if (fwrite_(&ElementSize, 4u, 1u, fp) != 1)
					{
						out_msg("fwrite");
						return -1;
					}
					if (fwrite_(par->p_value.p_str, ElementSize, 1u, fp) != 1)
					{
						out_msg("fwrite");
						return -1;
					}
				}
			}
		}
	}
	/*
	* 写第2部分
	*/
	for (p_sym_1 = symtab[p_sym_node.s_level]; p_sym_1 && !STOP; p_sym_1 = p_sym_1->s_next)
	{
		if (p_sym_1->s_next
			&& p_sym_1->s_next->s_level
			&& p_sym_1->s_next->s_level >= symtab[p_sym_node.s_level]->s_level
			&& (p_sym_1->s_next->s_level != symtab[p_sym_node.s_level]->s_level || p_sym_1->s_next->s_class != 1))
		{
			if (fwrite_(p_sym_1, 0x1Cu, 1u, fp) != 1)
			{
				out_msg("fwrite");
				return -1;
			}
		}
		else
		{
			/*
			* 参数结束，到另一个section
			*/
			if (fwrite_(p_sym_1, 0x18u, 1u, fp) != 1)
			{
				out_msg("fwrite");
				return -1;
			}
			if (fwrite_(&zero, 4u, 1u, fp) != 1)// 将链表的next = 0，表示结束
			{
				out_msg("fwrite");
				return -1;
			}
			STOP = 1;
		}
		ElementSize = strlen(p_sym_1->s_type) + 1;
		if (fwrite_(&ElementSize, 4u, 1u, fp) != 1)
		{
			out_msg("fwrite");
			return -1;
		}
		if (fwrite_(p_sym_1->s_type, ElementSize, 1u, fp) != 1)
		{
			out_msg("fwrite");
			return -1;
		}
		for (par = p_sym_1->s_param; par; par = par->p_next)
		{
			if (fwrite_(par, 0x10u, 1u, fp) != 1)
			{
				out_msg("fwrite");
				return -1;
			}
			if (par->p_type == 259)
			{
				ElementSize = strlen(par->p_value.p_str) + 1;
				if (fwrite_(&ElementSize, 4u, 1u, fp) != 1)
				{
					out_msg("fwrite");
					return -1;
				}
				if (fwrite_(par->p_value.p_str, ElementSize, 1u, fp) != 1)
				{
					out_msg("fwrite");
					return -1;
				}
			}
		}
	}
	return 0;
}

int __cdecl pdf_cppout(char* p_file)
{
	struct symtab_t* psym2; 
	int inited; 
	struct param_t* p_param_;
	struct symtab_t* s_next; 
	struct param_t* p_next; 
	FILE* Stream; 

	if (pdfsym_root[1].s_next)
	{
		pdfsym_root[1].s_curr = &pdfsym_root[1];
		s_next = pdfsym_root[1].s_next;
		pdfsym_root[1].s_next = 0;
		while (s_next->s_level)
		{
			if (s_next->s_type)
				free(s_next->s_type);
			if (s_next->s_param)
			{
				p_param_ = s_next->s_param;
				while (p_param_)
				{
					if (p_param_->p_type == 259)
						free(p_param_->p_value.p_str);
					p_next = p_param_->p_next;
					free(p_param_);
					p_param_ = 0;
					if (p_next)
						p_param_ = p_next;
				}
			}
			psym2 = s_next->s_next;
			free(s_next);
			s_next = 0;
			if (psym2)
				s_next = psym2;
		}
	}
	init_sym_table();
	p_symtab_curr = &pdfsym_root[1];
	s_level = 1;
	if (!strlen(p_file))
		return 0;
	/*
	* 返回 0  OK
	* 编译成符号文件,在这里直接返回，不再编译，直接使用已经编译好的文件
	*/
	// return 4;
	//ignore_compile
	strcpy(pdf_yyout_file, "yylex_err.txt");
	yyout = (DWORD)fopen(pdf_yyout_file, "wb");

	inited = exec_pdf_cppout(p_file);
	if (inited)
		return inited;
	int result = yyparse();
	fclose((FILE*)yyout);
	if (result)
		return result;

	fclose((FILE*)yyin);

	Stream = fopen_(pdfsym_file, "wb");
	if (!Stream)
		return -1;
	/*
	* 写符号文件
	*/
	result = write_sym_file(Stream);
	fclose_(Stream);
	return result;
}

char lv0[] = "Level0";
char lv1[] = "Level1";

void init_sym_table()
{
	int result;
	int i;
	
	pdfsym_root[0].s_type = lv0;
	pdfsym_root[0].s_class = 0;
	pdfsym_root[0].s_level = 0;
	pdfsym_root[0].s_param = 0;
	pdfsym_root[0].s_curr = pdfsym_root;
	pdfsym_root[0].s_prev = 0;
	pdfsym_root[0].s_next = &pdfsym_root[1];
	pdfsym_root[1].s_type = lv1;
	pdfsym_root[1].s_class = 1;
	pdfsym_root[1].s_level = 1;
	pdfsym_root[1].s_param = 0;
	pdfsym_root[1].s_curr = &pdfsym_root[1];
	pdfsym_root[1].s_prev = 0;
	pdfsym_root[1].s_next = pdfsym_root;
	pdfsym_list.first = &pdfsym_root[0];
	pdfsym_list.curr = &pdfsym_root[1];
	symtab_next.s_type = 0;
	symtab_next.s_class = 0;
	symtab_next.s_level = 0;
	symtab_next.s_param = 0;
	symtab_next.s_curr = &symtab_next;
	symtab_next.s_prev = 0;
	symtab_next.s_next = 0;
	p_symtab_curr = &pdfsym_root[1];
	s_level = 1;
	for (i = 0; i < 20; ++i)
	{
		symtab[i] = &pdfsym_root[0];
		result = i + 1;
	}
	symtab_inited = 1;
}


char* __cdecl tolower_(char* str)
{
	int i;

	for (i = 0; str[i]; ++i)
		str[i] = tolower(str[i]);
	return str;
}
struct symtab_t* __cdecl find_symbol_by_type(symtab_t* p_start, symtab_t* p_next, char* sym_name)
{
	char sym_type[44];
	struct symtab_t* psym = 0;

	psym = 0;
	while (!psym
		&& p_next
		&& (p_next->s_level > p_start->s_level || (p_next->s_level == p_start->s_level && p_next->s_class != 1) ))
	{
		strcpy(sym_type, p_next->s_type);
		tolower_(sym_type);
		if (p_next->s_level == p_start->s_level + 1 && 
			p_next->s_class == 1
			&& (!sym_name && strncmp(sym_type, str_crt, 4u)
				|| sym_name 
				&& (!strcmp(sym_type, sym_name) || !strncmp(sym_type, p_sym_node.name, 4u) && !strcmp(&sym_type[4], sym_name))))
		{
			psym = p_next;
		}
		p_next = p_next->s_next;
	}

	if (psym)
	{
		p_sym_node.s_level = p_start->s_level;
		add_to_table(psym);
	}
	return psym;
}
/*
* back - 1 向后查找
*/
char* __cdecl get_next_section(const char* symbol_name, int back)
{
	char src_type[40]; 
	int OK; 
	char s_type[44]; 
	int level;
	symtab_t* psym;
	symtab_t* p_next = 0;
	psym = 0;
	OK = 0;
	level = p_sym_node.s_level;
	if (symbol_name)
	{
		strcpy(src_type, symbol_name);
		tolower_(src_type);
		p_next = symtab[p_sym_node.s_level];
		while (level > 0 && !OK)
		{
			strcpy(s_type, p_next->s_type);
			tolower_(s_type);
			if (!strcmp(s_type, src_type)
				|| !strncmp(s_type, p_sym_node.name, 4u) && !strcmp(&s_type[4], src_type))
			{
				OK = 1;
				p_sym_node.s_level = --level;
				psym = find_symbol_by_type(symtab[level], symtab[level]->s_curr->s_next, src_type);
			}
			p_next = symtab[--level];
		}
		if (!OK)
		{
			level = p_sym_node.s_level;
			p_next = symtab[p_sym_node.s_level];
			while (level > 0 && !psym)
			{
				psym = find_symbol_by_type(p_next, p_next->s_next, src_type);
				p_next = symtab[--level];
			}
		}
	}
	else if (back)
	{
		psym = find_symbol_by_type(symtab[p_sym_node.s_level], symtab[p_sym_node.s_level]->s_curr->s_next, 0);
	}else
	{
		--p_sym_node.s_level;
		psym = find_symbol_by_type(symtab[p_sym_node.s_level], symtab[p_sym_node.s_level]->s_curr->s_next, 0);
	}
	if (!psym)
		return 0;
	p_symtab_curr = psym;
	strcpy(s_type, psym->s_type);
	char *l_type = tolower_(s_type);
	if (!strncmp(l_type, p_sym_node.name, 4u))
		return psym->s_type + 4;
	else
		return psym->s_type;
}

int __cdecl find_symbol_by_token(const char* name, int max_proc_num, int token_type, param_u* par, int* mask)
{
	symtab_t* p_symtab,*p_prev; 
	struct param_t* s_param;
	int STOP1; 
	int n_par_1, n_par;
	char Destination[40];
	char Str1[44];
	int p_type;
	int found;

	s_param = 0;
	p_type = p_sym_node.s_level;
	found = 0;
	STOP1 = 0;
	n_par_1 = 0;
	if (name)
	{
		/*
		* 首先向后搜索
		*/
		strcpy(Destination, name);
		tolower_(Destination);
		for (p_symtab = symtab[p_sym_node.s_level]->s_next;
			!found
			&& p_symtab
			&& (p_symtab->s_level > symtab[p_sym_node.s_level]->s_level
				|| p_symtab->s_class == 2 && p_symtab->s_level == symtab[p_sym_node.s_level]->s_level);
			p_symtab = p_symtab->s_next)
		{
			if (p_symtab->s_class == 2 && p_symtab->s_level == symtab[p_sym_node.s_level]->s_level)
			{
				strcpy(Str1, p_symtab->s_type);
				tolower_(Str1);
				if (!strcmp(Str1, Destination)
					|| !strncmp(Str1, p_sym_node.name, 4u) && !strcmp(&Str1[4], Destination))
				{
					found = 1;
					//p_sym_node.p_sym = p_symtab;
					s_param = p_symtab->s_param;
				}
			}
		}
		/*
		* 向后搜索未找到，则开始向前
		*/
		if (!found)
		{
			for (p_symtab = symtab[--p_type]; p_symtab->s_level; p_symtab = symtab[p_type])
			{
				for (p_prev = p_symtab->s_curr->s_prev;
					!found && (p_prev->s_level > p_symtab->s_level || p_prev->s_level == p_symtab->s_level && p_prev->s_class == 2);
					p_prev = p_prev->s_prev)
				{
					if (p_prev->s_class == 2 && p_prev->s_level == p_symtab->s_level)
					{
						strcpy(Str1, p_prev->s_type);
						tolower_(Str1);
						if (!strcmp(Str1, Destination)
							|| !strncmp(Str1, p_sym_node.name, 4u) && !strcmp(&Str1[4], Destination))
						{
							found = 1;
							s_param = p_prev->s_param;
						}
					}
				}
				--p_type;
			}
		}
	}
	else
	{
		found = 1;
		s_param = p_sym_node.p_par;
	}
	if (!found || max_proc_num)
	{
		if (found)
		{
			n_par_1 = 0;
			*mask = 0;
			while (n_par_1 < max_proc_num && s_param && !STOP1)
			{
				switch (token_type)
				{
				case 257:
					if (s_param->p_type == 257)
					{
						par[n_par_1].p_int = s_param->p_value.p_int;
						p_sym_node.p_par = s_param->p_next;
						++n_par_1;
					}
					else if (s_param->p_type == 258)
					{
						par[n_par_1].p_real = s_param->p_value.p_real;
						*mask |= 8u;
						p_sym_node.p_par = s_param->p_next;
						++n_par_1;
					}
					else
					{
						STOP1 = 1;
						*mask |= 0x10u;
						p_sym_node.p_par = s_param;
					}
					break;
				case 258:
					if (s_param->p_type == 258)
					{
						par[n_par_1] = s_param->p_value;
						p_sym_node.p_par = s_param->p_next;
						++n_par_1;
					}
					else if (s_param->p_type == 257)
					{
						par[n_par_1].p_real = (double)s_param->p_value.p_int;
						*mask |= 0x20u;
						p_sym_node.p_par = s_param->p_next;
						++n_par_1;
					}
					else
					{
						STOP1 = 1;
						*mask |= 0x40u;
						p_sym_node.p_par = s_param;
					}
					break;
				case 259:
					if (s_param->p_type == 259)
					{
						par[n_par_1].p_str = s_param->p_value.p_str;
						p_sym_node.p_par = s_param->p_next;
						++n_par_1;
					}
					else
					{
						STOP1 = 1;
						if (s_param->p_type == 257)
							*mask |= 2u;
						else
							*mask |= 4u;
						p_sym_node.p_par = s_param;
					}
					break;
				}
				s_param = s_param->p_next;
			}
		}
		if (s_param)
			*mask |= 1u;
		if (!found)
			return -1;
		return n_par_1;
	}
	else
	{
		n_par = 0;
		*mask = 0;
		while (s_param)
		{
			++n_par;
			s_param = s_param->p_next;
		}
		return n_par;
	}
}
int pipefd[2];

int __cdecl exec_display_process(char* szHwnd, char* szwindow_no, char* hostname, char* geometry, const char* iconic)
{

	param_u params[20]; 
	const char* argv[25]; 

	int stats; 
	char msg[512]; 
	char szpipe_read[80];
	char Buffer[32];

	char* program_name = 0;
	int ndisplay = 0;
	int ngeom = 0;
	int niconic = 0;
	int i = 0;
	if (find_symbol_by_token("program", 1, 259, params, &stats) == 1)
	{
		argv[0] = params[0].p_str;

		argv[2] = szHwnd;
		argv[3] = "WINDERZ";
		argv[4] = szwindow_no;
		i = 5;
		int n_par = find_symbol_by_token("Arguments", 20, 259, params, &stats) + 5;
		if (n_par > 5)
		{
			param_u* par = params;
			do
			{
				argv[i] = (const char*)par->p_str;
				if (!strcmp(par->p_str, "-display"))
					ndisplay = i + 1;
				if (!strcmp(par->p_str, "-geometry"))
					ngeom = i + 1;
				if (strstr(par->p_str, "iconic"))
					niconic = i;
				++par;
				++i;
			} while (i < n_par);

		}
		if (hostname)
		{
			if (ndisplay)
			{
				argv[ndisplay] = hostname;
			}
			else
			{
				argv[i] = "-display";
				i = i + 1;
				argv[i] = hostname;
				i = i + 1;
			}
		}
		if (geometry)
		{
			if (ngeom)
			{
				argv[ngeom] = geometry;
			}
			else
			{
				argv[i] = "-geometry";
				i = i + 1;
				argv[i] = geometry;
				i = i + 1;
			}
		}
		if (iconic)
		{
			sprintf(Buffer, "*iconic: %s", iconic);
			if (niconic)
			{
				argv[niconic] = Buffer;
			}
			else
			{
				argv[i] = "-xrm";
				i = i + 1;
				argv[i] = Buffer;
				i = i + 1;
			}
		}
		argv[i] = 0;
		/*
		*  管道 IPC 通讯过程
		* 1. 父进程调用pipe开辟管道，得到两个文件描述符指向管道的两端。 pipefd[0]= 3 pipefd[1] = 4
		* 2. 父进程调用fork创建子进程，那么子进程也有两个文件描述符指向同一管道。 
		* 3. 父进程关闭管道读端 pipefd[0]，子进程关闭管道写端 pipefd[1]。
		* 父进程可以往管道里写，子进程可以从管道里读，管道是用环形队列实现的，数据从写端流入从读端流出，这样就实现了进程间通信。
		*/
		//int result = _pipe(pipefd, 0x400u, 0x8000);
		int result = pipe(pipefd);
		if (result == -1)
		{
			out_msg("pipe");
		}
		else
		{
			set_exec_mode(0);
			sprintf(szpipe_read, "%d", pipefd[0]);
			intptr_t osfhandle = get_osfhandle_(pipefd[0]);
			sprintf(szpipe_read, "%d", osfhandle);
			argv[1] = szpipe_read;
			int pid_ = _setjmp(windux_fork_jump_buf);
			windux_fork();
			if (pid_ == -1)
			{
				out_msg("fork");
			}
			else if (pid_)
			{
				/*
				* 在父进程中
				*/
				sprintf(msg, "%s: Starting Display Process: [PID = %d]:\n %s ", procName, pid_, argv[0]);
				const char *pa = argv[5];
				int j = 5;
				if (argv[5])
				{
					do
					{
						int len = strlen(pa) + 1;
						char *p = &msg[-1];
						while (*++p)
							;
						qmemcpy(p, pa, len);
						p = &msg[-1];
						while (*++p)
							;
						pa = argv[++j];
						*(_WORD*)p = 32;
					} while (pa);
	
				}
				cls2_printf("%s\n", msg);
				// 关闭读取
				result = windux_close(pipefd[0]);
				if(result == -1)
				{
					out_msg("close");
				}
				else
				{
					/*
					* 把数据写给子进程
					* 数据包括进程的参数，标题 以及相关编译好的符号
					*/

					FILE* f = fdopen_(pipefd[1], "wb");
					int e = errno;
					if (f == NULL)
					{
						sprintf(msgBuffer, "fdopen failed : %d %s\n", e, strerror(errno));
						cls2_perror(msgBuffer);
						return pid_;
					}

					if(write_sym_file(f) == -1)
					{
						sprintf(msgBuffer, "%s: write pipe error", "dpymgr");
						cls2_perror(msgBuffer);
						return -1;
					}else
					{
						fclose_(f);
						register_resource(1, pid_);
						return pid_;
					}
				}
			}
			else
			{
				/*
				* 在子进程中
				* 关闭写
				*/

				//result = windux_close(pipefd[1]);
				//if (result == -1)
				//{
				//	out_msg("close");
				//	windux_exit(1);
				//}
				windux_execvp(argv[0], argv);
				out_msg("exec");
				MSGR_P256 msgp;
				msgp.mtype = 1;
				msgp.msg_ident = 0x75;
				*(_DWORD*) msgp.msg_data = atol(szwindow_no);
				if (msgsnd(msgqid_dpymgr, &msgp, 8, 0) == -1)
					cls2_printf("dpymgr: msgsnd failed.\n");
			}
		}
	}
	else
	{
		fprintf(stdout, "dpymgr: Can't find program name.\n");
	}
	return -1;
}

void init_dpymenu()
{
	int priv_msgq; 
	char Buffer[511]; 
	char szinfo[1000]; 
	char szinfo1[300];
	const char* pdf_files[4];
	char pdfFileName[256];
	char dpyFileName[256]; 
	char finded_incude_path[256];
	char str_dpymgr_msgqid[12];
	int no_octinitlaunch = 0;
	char* p_pdffile = 0;
	param_u params[20];
	int stats;
	struct _stat32 fs_pdf;
	if (!_stat32(".autostart_pdf.txt", &fs_pdf))
	{
		_putenv_s("PDF_SYS_WORK_DIR", getenv("SYS_WORK_DIR"));
		unlink(".autostart_pdf.txt");
	}
	p_octname = getenv("OCT_NAME");
	if (getenv("ACQ_DPY_DIR"))
	{
		strcpy(dpyFileName, octname);
		char* p = &dpyFileName[-1];
		while (*++p)
			;
		strcpy(p, ".dpy");
		if (_stat32(dpyFileName, &fs_pdf))
		{
			char* dpy_file = Du_FindPdf(dpyFileName);
			strcpy(dpyFileName, dpy_file);
		}
		strcpy(finded_incude_path, p_octname);
		p = &finded_incude_path[-1];
		while (*++p)
			;
		strcpy(p, ".oct");
		char *p_octfile = Du_FindPdf(finded_incude_path);
		if (!strlen(p_octfile))
		{
			strcpy(finded_incude_path, octname);
			p = &finded_incude_path[-1];
			while (*++p)
				;
			strcpy(p, ".oct");
			p_octfile = Du_FindPdf(finded_incude_path);
			if (!strlen(p_octfile))
			{
				strcpy(finded_incude_path, octname);
				while (*++p)
					;
				strcpy(p, ".pdf");
				p_octfile = Du_FindPdf(finded_incude_path);
			}
			if (!strlen(p_octfile))
				finded_incude_path[0] = 0;
		}
		if (strlen(p_octfile))
		{
			strcpy(pdfFileName, finded_incude_path);
			strcpy(finded_incude_path, p_octfile);
		}
	}
	else
	{
		cls2_printf("%s: Can't find ACQ_DPY_DIR in environment\n", procName);
		finded_incude_path[0] = 0;
	}
	if (finded_incude_path[0] == 0 && dpyFileName[0] == 0)
	{
		if (!strcmp(octname, p_octname))
			cls2_printf("%s: Warning. Can't find PDF '%s.dpy', '%s.oct' or '%s.pdf'\n", procName, octname, octname, octname);
		else
			cls2_printf(
				"%s: Warning. Can't find PDF '%s.dpy', '%s.oct', '%s.oct' or '%s.pdf'\n",
				procName,
				octname,
				p_octname,
				octname,
				octname);
		not_found = 1;
	}
	else
	{
		p_pdffile = dpyFileName;
		if (!strlen(dpyFileName))
		{
			p_pdffile = finded_incude_path;
		}
		FILE* fp = fopen(p_pdffile, "r");
		if (fp)
		{
			fclose(fp);
			if (strlen(finded_incude_path))
			{
				pdf_files[0] = ".oct";
				pdf_files[1] = ".log";
				pdf_files[2] = ".crvs";
				pdf_files[3] = ".rpt";
				char* p_workdir = getenv("SYS_WORK_DIR");
				for (int i = 0; i < 4; ++i)
				{
					char* p_extname = strrchr(pdfFileName, '.');
					if (p_extname)
					{
						strcpy(p_extname, pdf_files[i]);

						char* pp = Du_FindPdf(pdfFileName);
						if (pp)
						{
							if (!strstr(pp, p_workdir))
							{
								if (check_pdf_file(pdfFileName))
									_chmod(pdfFileName, 0x80);
								sprintf(Buffer, "cp -f \"%s\" .", pp);
								windux_system(Buffer);
							}
						}
					}
				}

			}
			cls2_printf("\tdpymgr: Processing %s\n", p_pdffile);
			int L = strlen(dpyFileName);
			if (!L)
			{
				strcpy(dpyFileName, &finded_incude_path[L]);

				convert_to_dpy(procName, dpyFileName, octname);
				p_pdffile = dpyFileName;
			}
			strcpy(pdfsym_file, "pdfsym.txt");
			strcpy(pdf_cppout_file, "pdf_cppout.txt");

			if (pdf_cppout(p_pdffile) <= 4)
			{
				pdf_load_sym(0, 1);
				sym_succeessed = 1;
			}
			else
			{
				cls2_printf("%s: Can't process %s\n", procName, p_pdffile);
			}
			char* hostname = 0;
			char* geometry = 0;
			char* iconic = 0;
			char szwindow_no[4] = {0};
			char *p_acq_dpyhost = getenv("ACQ_DPY_HOST");
			no_octinitlaunch = 0;
			if (sym_succeessed && proc_normal && get_next_section("section", 0))
			{
				do
				{
					sprintf(szwindow_no, "%d", window_no);
					int find_display = 0;
					int find_proc_host = 0;
					int find_disp_name = 0;
					int n_par = find_symbol_by_token("Arguments", 20, 259, params, &stats);
					if (n_par > 0)
					{
						for (int i = 0; 0 < n_par - 1; ++i)
						{
							if (find_display)
								break;
							if (!strcmp(params[i].p_str, "-display"))
							{
								find_display = 1;
								if (strcmp(params[i + 1].p_str, "PROC_HOST") == 0 && strcmp(p_acq_dpyhost, pCmdLine) == 0)
									find_proc_host = 1;
							}
						}
					}
					if (find_symbol_by_token("DisplayName", 1, 259, params, &stats) > 0)
					{
						strcpy(winds[window_no].dpy_host_name, params[0].p_str);
						hostname = winds[window_no].dpy_host_name;
						if (strcmp(winds[window_no].dpy_host_name, "PROC_HOST") || (find_disp_name = 1, strcmp(p_acq_dpyhost, pCmdLine)))
						{
							find_disp_name = find_proc_host;
						}
					}else
						find_disp_name = find_proc_host;
		
					if (!find_display && strcmp(p_acq_dpyhost, pCmdLine))
						find_disp_name = 1;
					if ((!strcmp(p_acq_dpyhost, "NONE") || find_disp_name) && find_symbol_by_token("OCTInitLaunch", 1, 259, params, &stats) > 0)
					{
						no_octinitlaunch = 1;
						if (!stricmp(params[0].p_str, "yes") || !stricmp(params[0].p_str, "true"))
						{
							if (!find_display)
								hostname = nullstring;
							if (find_symbol_by_token("IncludedFileName", 1, 259, params, &stats) > 0)
							{
								strcpy(winds[window_no].window_title, params[0].p_str);
		
							}
							if (find_symbol_by_token("Geometry", 1, 259, params, &stats) <= 0)
								geometry = nullstring;
							else
								geometry = strdup(params[0].p_str);
							if (find_symbol_by_token("Iconic", 1, 259, params, &stats) <= 0)
								iconic = nullstring;
							else
								iconic = strdup(params[0].p_str);

							winds[window_no].status = 1;
							winds[window_no].window_no = -1;
							int pid = exec_display_process(str_dpymgr_hwnd, szwindow_no, hostname, geometry, iconic);
							winds[window_no].pid = pid;
							if (pid != -1)
								++window_no;
						}
					}
				} while (get_next_section("section", 0));
			}
		}
		else
		{
			cls2_printf("%s: Can't open %s\n", procName, p_pdffile);
		}
	}
	if (window_no == start_win_no)
	{
		sprintf(
			szinfo,
			"Warning: No default displays will be launched.%cDisplay windows must be started manually from%cthe Display Launcher.%cReason: ",
			11,
			11,
			11);
		if (p_pdffile && strlen(p_pdffile))
		{
			if (no_octinitlaunch == 0)
				sprintf(
					szinfo1,
					"No OCTInitLaunch directives in file%c%s%cPossibly from a pre-ECLIPS 6.1U2 release",
					11,
					p_pdffile,
					11);
			else
				sprintf(szinfo1, "Unknown. Check file%c%s", 11, p_pdffile);
		}
		else
		{
			strcpy(szinfo1, "No suitable .oct or .dpy file found");
		}
		int l = strlen(szinfo1) + 1;
		char *p = &szinfo[-1];
		while (*++p)
			;
		qmemcpy(p, szinfo1, l);

		cls2_printf("%s : Warning: No default displays will be launched.\n", procName);
		/*
		* 注释掉
		*/
		//sprintf(cmdline, "%s %s \"%s\\dpysenderr\" %d \"%s\" \"%s\" &", "remsh", pCmdLine, p_bin_dir, ei_no, ipcfile, szinfo);
		//windux_system(cmdline);
		not_found = 1;
	}
	priv_msgq = create_priv_msgq();
	msgqid_dpymgr = priv_msgq;
	if (priv_msgq == -1)
	{
		cls2_printf("%s: msgget failed\n", procName);
		quit(1);
		priv_msgq = msgqid_dpymgr;
	}
	sprintf(str_dpymgr_msgqid, "%d", priv_msgq);

	const char* argv2[6];
	argv2[0] = (char*)"dpymenu";
	argv2[1] = str_dpymgr_msgqid;
	argv2[3] = str_dpymgr_hwnd;
	char w_no[4];
	if (strlen(finded_incude_path))
	{
		argv2[4] = finded_incude_path;
		argv2[5] = 0;
	}
	else
	{
		argv2[4] = 0;
	}

	for (int j = 0; proc_normal; ++j)
	{
		if (j >= start_win_no)
			break;
		sprintf(w_no, "%d", j);
		argv2[2] = w_no;

		winds[j].pid = _setjmp(windux_fork_jump_buf);
		windux_fork();
		if (!winds[j].pid)
		{
			windux_execvp("dpymenu", argv2);
			sprintf(msgBuffer, "%s: exec of dpymenu failed", procName);
			cls2_perror(msgBuffer);
			quit(1);
		}
		register_resource(1, winds[j].pid);
	}
	_beginthread((_beginthread_proc_type)refresh_thread, 0, 0);
	return;
}

int proc_dpy_msg()
{

	OUTBUF_HDR* p_outbuf_hdr; 
	FILE* fp_out = 0;
	char FileName[256];
	char outfilename[256];
	int i = 0,j = 0, siz;
	aif_frame_hdr_t* p_aif_frame_hdr = 0;
	aif_frame_t* p_aif_frame = 0;
	int b_data = 0;
	char* p = 0;
	if (!p_dpy_frame_ptr)
	{
		cls2_printf("%s: Initialization data NOT received. Exiting ...\n", procName);
		quit(1);

	}
	DPY_START_ACQ *p_startacq = (DPY_START_ACQ*)p_dpy_shm;
	DPY_START_ACQ* p_dpy_startacq = 0;
	p_outbuf_hdr = (OUTBUF_HDR*)p_dpy_frame_ptr;
	if (not_first_time)
	{
		int result = *(_DWORD*)p_dpy_frame_ptr;
		if (!result)
			return result;
		/*
		* 指向 OUTBUF_HDR 的指针
		*/
		p_dpy_frame_ptr = result + p_dpy_shm;
		p_outbuf_hdr = (OUTBUF_HDR*)(result + p_dpy_shm);
	}
	else
	{
		not_first_time = 1;
	}
	/*
	* 缓存区间的大小
	*/
	*(_DWORD*)(p_dpy_shm + 0x134) = p_dpy_frame_ptr - p_dpy_shm;
	if (p_dpy_frame_ptr)
	{

		while (1)
		{
			// aif_frame_info_t 
			int ptr = (int)((char*)p_dpy_shm + p_outbuf_hdr->data_offset + 8 );
			int code = p_outbuf_hdr->code - 2;
			last_block_id = p_outbuf_hdr->block_id;

			switch (code)
			{
			case 0:  // code 2
				// OUTBUF_HDR
				// tag      block_id data_offset code   data_size
				// 00000000 02000000 30941700 02000000 28010000
				p_dpy_startacq = (DPY_START_ACQ*)ptr;
				swap2((byte*)&p_dpy_startacq->workmode, 1);
				swap4((byte*)&p_dpy_startacq->log_dir_record, 1);
				swap2((byte*)&p_dpy_startacq->drive_mode, 1);
				p_startacq = (DPY_START_ACQ*)p_dpy_shm;
				p_startacq->workmode = p_dpy_startacq->workmode;
				p_startacq->log_dir_record = p_dpy_startacq->log_dir_record;
				p_startacq->drive_mode = p_dpy_startacq->drive_mode;
				strcpy(p_startacq->trip, p_dpy_startacq->trip);
				strcpy(p_startacq->timestamp, p_dpy_startacq->timestamp);
				strcpy(p_startacq->aff_filename, p_dpy_startacq->aff_filename);

				if (p_startacq->aff_filename[0])
				{
					char* p = p_startacq->aff_filename;
					do
					{
						if (*p == '/')
							*p = '\\';
						++p;
					} while (*p);
				}

				i = start_win_no;
				if (start_win_no >= window_no)
					break;

				do
				{
					if (winds[i].status == 0)
					{
						winds[i].status = 2;
						PostMessage((HWND)winds[start_win_no].hwnd, 0x402u, 0, 0);
						wait_for_response(i);
					}
					++i;
				} while (i < window_no);
				break;
			case 1: // code 3
				// OUTBUF_HDR
				// tag      block_id data_offset code   data_size
				// 00000000 03000000 6C951700 03000000 48590A00
				msg_dpy_curve_data(p_outbuf_hdr);
				break;
			case 2: // code 4
				p_startacq->aff_filename[0] = 0;
				i = start_win_no;
				if (start_win_no >= window_no)
					break;
				do
				{
					if (winds[i].status == 0)
					{
						winds[i].status = 2;
						PostMessage((HWND)winds[i].hwnd, 0x404u, 0, 0);
						wait_for_response(i);
					}
					++i;
				} while (i < window_no);
				break;
			case 3:// code 5
				b_data = 1;
				i = start_win_no;
				if (start_win_no < window_no)
				{
					do
					{
						if (winds[i].status == 0)
						{
							winds[i].status = 2;
							PostMessage((HWND)winds[i].hwnd, 0x405u, 0, 0);
							wait_for_response(i);
						}
						++i;
					} while (i < window_no);
				}
				quit(0);
				break;
			case 4: // code 6
				// OUTBUF_HDR
				// tag      block_id data_offset code   data_size
				// 00000000 01000000 2C860900 06000000  F00D0E00
				*(_DWORD*)(p_dpy_shm + 0x12C) = 0;
				p_aif_frame_hdr = (aif_frame_hdr_t*)(ptr);
				convert_aif_frame(p_aif_frame_hdr);
				p_aif_frame = (aif_frame_t*)((char*)p_aif_frame_hdr + 0x474);
				convert_aif_crv(p_aif_frame, p_aif_frame_hdr->file_hdr.num_frames);
				GetTempPath(0x100u, outfilename);
				sprintf(FileName, "%s%s%d%s", outfilename, "/OUTBUF", ei_no, ".hdr");
				sprintf(str_outbuf, "OUTBUF_HDR=%s", FileName);
				if (putenv(str_outbuf))
				{
					cls2_printf("%s: Could not add OUTBUF_HDR to environment\n", procName);
					return -1;
				}
				fp_out = fopen(FileName, "wb");

				if (fp_out)
				{
					if (fwrite(p_aif_frame_hdr, p_outbuf_hdr->data_size, 1u, fp_out) != 1)
						proc_normal = 0;
					fclose(fp_out);
					register_file(FileName);
				}
				else
				{
					proc_normal = 0;
					cls2_printf("%s: Could not open %s\n", procName, FileName);
				}
				strcpy(octname, p_aif_frame_hdr->file_hdr.oct);

				/*
				* 注释掉下一行，不用5700的显示界面
				*/
				init_dpymenu();

				break;
			case 5:
				swap4((byte*)&ptr, 1);
				j = 0;
				if (window_no > 0)
				{

					do
					{
						if (!winds[j].status)
						{
							winds[j].status = 2;
							PostMessage((HWND)winds[j].hwnd, 0x407u, *(int*)ptr, 0);
							wait_for_response(j);
						}
						++j;
					} while (j < window_no);
				}
				break;
			case 6:
				// 
				siz = p_outbuf_hdr->data_size / 4;
				i = 0;
				if (siz > 0)
				{
					int hdr_size = *(_DWORD*)(p_dpy_shm + 0x128); // = 0x144
					int dpy_cdss_num = (*(_DWORD*)(hdr_size + p_dpy_shm + 8) + 1);
					DPY_CURVE_INFO* p_crv_block = (DPY_CURVE_INFO*)((char*)p_dpy_shm
						+ hdr_size + 28 * dpy_cdss_num);
					do
					{
						int ele = ((int*)ptr)[i];
						swap4((byte*)&ele, 1);
						p_crv_block->active_num_ele = ele;
						++i;
						++p_crv_block;
					} while (i < siz);
	
				}

				i = start_win_no;
				if (start_win_no >= (int)window_no)
					break;
				do
				{
					if (!winds[i].status)
					{
						winds[i].status = 2;
						PostMessage((HWND)winds[i].hwnd, 0x408u, 0, 0);
						wait_for_response(i);
					}
					++i;
				} while (i < window_no);
				break;
			case 7:
				swap4((byte*)ptr, 1);
				i = 0;
				if (window_no > 0)
				{
					do
					{
						if (!winds[i].status)
						{
							winds[i].status = 2;
							PostMessage((HWND)winds[i].hwnd, 0x409u, *(int*)ptr, 0);
							wait_for_response(i);
						}
						++i;

					} while (i < window_no);
				}
				break;
			case 8:  // DPY_PERF_INFO ？
				p = (char*)(ptr + 8);
				swap4((byte*)ptr, 2);
				for (i = 0; i < *(int*)(ptr + 4); p += 16)
				{
					swap4((byte*)p, 4);
					++i;
				}

				i = 0;

				*(_DWORD*)(p_dpy_shm + 0x12C) = p_outbuf_hdr->data_offset + 8;
				if (window_no > 0)
				{
					do
					{
						if (!winds[i].status)
						{
							winds[i].status = 2;
							PostMessage((HWND)winds[i].hwnd, 0x40Au, *(_DWORD*)(p_dpy_shm + 0x12C), 0);
							wait_for_response(i);

						}
						++i;
					} while (i < window_no);
				}
				break;
			default:

				cls2_printf("%s: Invalid block_id %d: %s\n", procName, p_outbuf_hdr->block_id, get_current_time());
				break;
			}

			if (!p_outbuf_hdr->next_frame_off)
				return b_data;
			p_outbuf_hdr = (OUTBUF_HDR*)((char*)p_dpy_shm + p_outbuf_hdr->next_frame_off);
			p_dpy_frame_ptr = (int)p_outbuf_hdr;
			*(_DWORD*)(p_dpy_shm + 0x134) = (char*)p_outbuf_hdr - (char*)p_dpy_shm;
			if (!p_outbuf_hdr)
				return b_data;
		}
	}
	return b_data;
}

int launch_display_windows(int no)
{
	char szwindow_no[4]; 
	char Buffer[12]; 
	int win_no = 0,i = 0;
	sprintf(Buffer, "%d", (int)hWnd);

	strcpy(pdfsym_file, "pdfsym.txt");
	strcpy(pdf_cppout_file, "pdf_cppout.txt");
	/*
	* 以下先注释掉,进行编译，直接使用已经编译好的符号文件
	*/
	if (pdf_cppout(msgp.msg_data) > 4)
	{
		cls2_printf("%s: Can't process %s\n", procName, msgp.msg_data);
		return PostMessage((HWND)winds[no].hwnd, 0x43Au, 0, 0);
	}

	pdf_load_sym(0, 1);
	char*result = get_next_section("section", 0);
	if (result)
	{
		int end = 0;
		i = start_win_no;
		if (start_win_no >= window_no)
		{
		LABEL_11:
			if (window_no == 100)
			{
				cls2_printf("%s: Maximum of %d display windows already in use in this EI.  A window must be killed before another can be launched.\n", procName, 100);
				return 0;
			}
			win_no = window_no++;
			winds[win_no].status = -1;
		}
		else
		{
			while (!end)
			{
				if (winds[i].status == -1)
				{
					end = 1;
					win_no = i;
				}
				++i;
				if (i >= window_no)
				{
					if (end)
						break;
					goto LABEL_11;
				}
			}
		}
		sprintf(szwindow_no, "%d", win_no);
		winds[win_no].pid = exec_display_process(Buffer, szwindow_no, 0, 0, 0);

		if (winds[win_no].pid == -1)
		{
			return PostMessage((HWND)winds[no].hwnd, 0x43Au, 0, 0);
		}
		else
		{
			winds[win_no].status = 1;
			winds[win_no].window_no = no;
			char*p = strrchr(msgp.msg_data, '\\');
			if (p)
				p = p + 1;
			else
				p = msgp.msg_data;
			strcpy(winds[win_no].window_title, p);

			not_found = 0;
		}
	}
	return 0;
}

int WndMsgHandler(UINT message, WPARAM wParam, LPARAM lParam)
{
	int result = 0, win_no;
	int msgid = 0;
	win_no = ((message & 0xFF00u) - 0x301) >> 8;
	/*
	* 低8位是消息id
	*/
	msgid = (unsigned __int8)message;
	if (win_no == 99)
	{
		/*
		* win_no == 99 的话,表示是 dpybufmgr 发回的消息
		*/
		switch (msgid)
		{
		case 0x35:
			cls2_printf("WndMsgHandler: DPY_INIT_DONE %d\n", lParam);
			if (lParam == -1)
			{
				cls2_printf("%s: Could not open socket: %s\n", procName, get_current_time());
				quit(1);
			}

			register_resource(1, pid);
			return 0;
		case 0x63:
			if (!process_tag)
			{
				process_tag = 1;
				result = proc_dpy_msg();
				process_tag = 0;
				if (result)
					quit(0);
			}
			return msgid;
		case 0x64:
			p_dpy_frame_ptr = p_dpy_shm + *(_DWORD*)(p_dpy_shm + 0x134); // frame_off
			// p_dpy_shm + 0x144 -> DPY_INIT_HDR
			convert_dpy_init_hdr((DPY_INIT_HDR*)(p_dpy_shm + 0x144));
			return 0;
		}
	}
	int code = msgid - 50; // 0x3C & 0xFFFFFFCE = C
	if (win_no <= window_no)
	{
		switch (code)
		{
		case 0:
			if (winds[win_no].status == -1)
				result = cls2_printf("%s: Window %d [%s] RESUMED responding\n", procName, win_no, winds[win_no].window_title);
			winds[win_no].status = 0;
			break;
		case 2: // kill window 终止该进程
			cls2_printf("%s: Terminating Display Process: [PID = %d]\n", procName, winds[win_no].pid);
			kill(winds[win_no].pid, 9);

			winds[win_no].status = -1;
			PostMessage(hWnd, (win_no << 8) + 0x432, 0, 0);
			if (winds[win_no].window_no != -1)
				result = PostMessage((HWND)winds[winds[win_no].window_no].hwnd, 0x43Au, 0, 0);
			winds[win_no].window_no = -1;
			break;
		case 3: // 通过PostMessage终止进程
			winds[win_no].field_8 = lParam;
			winds[win_no].hwnd = (HWND)wParam;
			if (winds[win_no].window_no != -1)
				result = PostMessage((HWND)winds[winds[win_no].window_no].hwnd, 0x43Au, 0, 0);
			winds[win_no].window_no = -1;
			if (winds[win_no].status != -1)
				winds[win_no].status = 0;
			break;
		case 4: // new window 请求
			while (1)
			{
				result = recv_msg(&msgp, msgqid_dpymgr, 0, 0);
				if (result == -1)
					break;
				if (result <= 0)
					return result;
				if (msgp.msg_ident == 0x75)
				{
					// 终止进程
					win_no = *(_DWORD*)msgp.msg_data;
					kill(winds[win_no].pid, 9);
					wait(0);
					winds[win_no].status = -1;
					PostMessage((HWND)winds[win_no].hwnd, 0x43Au, 0, 0);
					winds[win_no].window_no = -1;
				}
				else if (msgp.msg_ident == 0x74)
				{
					// 启动进程
					return launch_display_windows(win_no);
				}
			}
			cls2_printf("%s: new window message failed: %s\n", procName, get_current_time());
			quit(1);
			break;
		case 9:
			if (wParam || lParam)
			{
				message = 0xE4 * win_no;
				winds[win_no].X = SLOWORD(wParam);
				winds[win_no].Y = SHIWORD(wParam);
				winds[win_no].width = SLOWORD(lParam);
				winds[win_no].height = SHIWORD(lParam);
				winds[win_no].Iconic = 0;
				winds[win_no].index = window_index++;
				strcpy(winds[win_no].dpy_host_name, "PROC_HOST");

				if (SLOWORD(wParam) > 1280)
				{
					winds[win_no].X = SLOWORD(wParam) - 1280;
				}
			}
			else
			{
				winds[win_no].Iconic = 1;
			}
			break;
		case 10:
			++heart_breaks;
			break;
		default:
			return result;
		}
	}
	return result;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message < 0x400)
		return DefWindowProc(hWnd, message, wParam, lParam);
	WndMsgHandler(message, wParam, lParam);
	return 0;

}
//
//FILE* yyin;
//int oldline, oldend, oldstart, oldlastc, oldlength;
//char yy_c_buf_p[604] = { 0 };
//char yytext[604] = { 0 };
//
//char* yytext_ptr = yy_c_buf_p;
//
//char* str_error_texts = 0;
//
//yy_save_block_tag* __cdecl yy_create_buffer(FILE* fp)
//{
//	yy_save_block_tag* p;
//
//	p = (yy_save_block_tag*)malloc(0x4CCu);
//	if (!p)
//		return 0;
//	p->oldfp = yyin;
//	p->oldline = oldline;
//	p->oldend = oldend;
//	p->oldstart = oldstart;
//	p->oldlastc = oldlastc;
//	p->oldleng = oldlength;
//	memcpy(p->savetext, yytext, 601u);
//	memcpy(p->savestate, yy_c_buf_p, 601u);
//	yyin = fp;
//	oldline = 1;
//	oldend = 0;
//	oldlength = 0;
//	oldstart = 0;
//	oldlastc = 10;
//	return p;
//}
//
//void __cdecl yy_delete_buffer(yy_save_block_tag* Block)
//{
//	if (Block)
//	{
//		yyin = (FILE*)Block->oldfp;
//		oldline = Block->oldline;
//		oldend = Block->oldend;
//		oldstart = Block->oldstart;
//		oldlastc = Block->oldlastc;
//		oldlength = Block->oldleng;
//		memcpy(yytext, Block->savetext, 0x259u);
//		memcpy(yy_c_buf_p, Block->savestate, 0x259u);
//		free(Block);
//	}
//}
//
//int input_c()
//{
//	int c; 
//
//	if (yytext_ptr >= yy_c_buf_p)
//	{
//		c = getc(yyin);
//	}
//	else
//	{
//		c = *yytext_ptr++;
//	}
//	oldlastc = c;
//	if (c == 10)
//		++oldline;
//	return c;
//}
//
//int yyinput()
//{
//	int C1,C2;
//
//	oldlength = 0;
//	do
//	{
//		C1 = input_c();
//		if (C1 == -1)
//			break;
//		if (C1 == '\n')
//			return 10;
//		if (C1 != '\r')
//		{
//			if (C1 == '\\')
//			{
//				C2 = input_c();
//				if (C2 != '\r' && C2 != '\n')
//				{
//					switch (C2)
//					{
//					case '\\':
//						yytext[oldlength++] = '\\';
//						break;
//					case 'n':
//						yytext[oldlength++] = '\n';
//						break;
//					case '"':
//						yytext[oldlength++] = '"';
//						break;
//					default:
//						yytext[oldlength++] = C1;
//						if (C2 != -1)
//							yytext[oldlength++] = C2;
//						break;
//					}
//				}
//			}
//			else
//			{
//				yytext[oldlength++] = C1;
//			}
//		}
//	} while (C1 != '"');
//	if (oldlength > 0)
//		--oldlength;
//	yytext[oldlength] = 0;
//	return 0;
//}
//
//char* get_error_text()
//{
//	char* result;
//
//	char Destination[1004]; 
//
//	strcpy(Destination, yytext);
//	if (str_error_texts)
//		free(str_error_texts);
//	str_error_texts = (char*)calloc(oldlength, 1u);
//
//	if (str_error_texts)
//	{
//		result = strtok(Destination, "# \t");
//		oldline = atoi(result);
//		result = strtok(0, "\r\n");
//		return strcpy(str_error_texts, result);
//	}
//	return str_error_texts;
//}

char* __cdecl alloc_copy(char* Str)
{
	size_t l; 
	char* Destination; 

	l = strlen(Str);
	Destination = (char*)my_alloc(1u, l + 1);
	strcpy(Destination, Str);
	return Destination;
}
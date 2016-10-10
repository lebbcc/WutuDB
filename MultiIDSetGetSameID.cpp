#include "MultiIDSetGetSameID.h"
#include <windows.h>
#include <Shlwapi.h>
#include <cstdint>

typedef __int64 multidsetidtype;

//succ return 0;
int MultiIDSetGetSameID(std::deque<std::string*> &idsetdata, std::string *out_ids, int idsize)
{
	
	struct idsinfopath
	{
		int type;
		const char *idsbuf, *pidsbuf;
		int   idsbufsize;
		HANDLE idsfile;
		char *idsfilebuf, *pidsfilebuf;
		int idsfilebufdatasize;
		multidsetidtype idsfilebufstart,idsfilesize;
	};
	std::deque<idsinfopath> idsetdata2;
	for (int i = 0; i < idsetdata.size(); i++)
	{
		idsinfopath idsinfopa;
		if (PathFileExistsA(idsetdata[i]->c_str()))
		{
			idsinfopa.type = 1;
			idsinfopa.idsfilebufdatasize = 0;
			idsinfopa.idsfilebufstart = 0;
			idsinfopa.idsfile = CreateFileA(idsetdata[i]->c_str(), GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,0,OPEN_EXISTING,0,0);
			DWORD sizehi=0;
			idsinfopa.idsfilesize = GetFileSize(idsinfopa.idsfile, &sizehi);
			idsinfopa.idsfilesize |= ((uint64_t)sizehi << 32);
		}
		else{
			idsinfopa.type = 0;
			idsinfopa.idsbuf = idsetdata[i]->c_str();
			idsinfopa.pidsbuf = idsinfopa.idsbuf;
		}
		idsetdata2.push_back(idsinfopa);
	}

	while(true)
	{
		//找到一个最大的
		bool batmax = false;
		multidsetidtype id=0;
		for (int i = 0; i < idsetdata2.size(); i++)
		{
			if (idsetdata2[i].type == 0)
			{
				if (idsize == 4)
				{
					if (idsetdata2[i].pidsbuf+4 - idsetdata2[i].idsbuf>idsetdata2[i].idsbufsize){ batmax = true; break; }
					if (((uint32_t*)idsetdata2[i].pidsbuf)[0]>id)
					{
						id = ((uint32_t*)idsetdata2[i].pidsbuf)[0];
					}
				}
				else if (idsize == 8)
				{
					if (idsetdata2[i].pidsbuf + 8 - idsetdata2[i].idsbuf>idsetdata2[i].idsbufsize){ batmax = true; break; }
					if (((uint64_t*)idsetdata2[i].pidsbuf)[0]>id)
					{
						id = ((uint64_t*)idsetdata2[i].pidsbuf)[0];
					}
				}
			}
			else if (idsetdata2[i].type == 1)
			{
				if (idsetdata2[i].idsfilebufdatasize == 0 || idsetdata2[i].pidsfilebuf + idsize - idsetdata2[i].idsfilebuf > idsetdata2[i].idsfilebufdatasize)
				{
					if (idsetdata2[i].idsfilebufstart + idsetdata2[i].idsfilebufdatasize == idsetdata2[i].idsfilesize)
					{
						int readsize = 0;
						if (idsetdata2[i].idsfilesize - idsetdata2[i].idsfilebufstart < 10 * 1024 * 1024)
						{
							readsize = idsetdata2[i].idsfilesize - idsetdata2[i].idsfilebufstart;
						}
						else{
							readsize = 10 * 1024 * 1024;
						}
						if (readsize == 0){ batmax = true; break; }
						DWORD readcnt;
						ReadFile(idsetdata2[i].idsfile, idsetdata2[i].idsfilebuf, readsize,&readcnt, 0);
						idsetdata2[i].idsfilebufdatasize = readsize;
						idsetdata2[i].idsfilebufstart += readsize;
						idsetdata2[i].pidsfilebuf = idsetdata2[i].idsfilebuf;
					}
				}

				if (idsize == 4)
				{
					if (idsetdata2[i].pidsfilebuf - idsetdata2[i].idsfilebuf){}
					if (((uint32_t*)idsetdata2[i].pidsfilebuf)[0]>id)
					{
						id = ((uint32_t*)idsetdata2[i].pidsfilebuf)[0];
					}
				}
				else if (idsize == 8)
				{
					if (((uint64_t*)idsetdata2[i].pidsfilebuf)[0]>id)
					{
						id = ((uint64_t*)idsetdata2[i].pidsfilebuf)[0];
					}
				}
			}
		}
		if (batmax)break;

		//全部前进到大于等于处
		for (int i = 0; i < idsetdata2.size(); i++)
		{
			if (idsetdata2[i].type == 0)
			{
				if (idsize == 4)
				{
					if (idsetdata2[i].pidsbuf + 4 - idsetdata2[i].idsbuf>idsetdata2[i].idsbufsize){ batmax = true; break; }
					if (idsetdata2[i].pidsbuf - idsetdata2[i].idsbuf<idsetdata2[i].idsfilebufdatasize && ((uint32_t*)idsetdata2[i].pidsbuf)[0]<id)
					{
						idsetdata2[i].pidsbuf += 4;
					}
				}
				else if (idsize == 8)
				{
					if (idsetdata2[i].pidsbuf + 8 - idsetdata2[i].idsbuf>idsetdata2[i].idsbufsize){ batmax = true; break; }
					if (idsetdata2[i].pidsbuf - idsetdata2[i].idsbuf<idsetdata2[i].idsfilebufdatasize && ((uint64_t*)idsetdata2[i].pidsbuf)[0]<id)
					{
						idsetdata2[i].pidsbuf += 8;
					}
				}
			}
			else if (idsetdata2[i].type == 1)
			{
				if (idsetdata2[i].idsfilebufdatasize == 0 || idsetdata2[i].pidsfilebuf + idsize - idsetdata2[i].idsfilebuf > idsetdata2[i].idsfilebufdatasize)
				{
					if (idsetdata2[i].idsfilebufstart + idsetdata2[i].idsfilebufdatasize == idsetdata2[i].idsfilesize)
					{
						int readsize = 0;
						if (idsetdata2[i].idsfilesize - idsetdata2[i].idsfilebufstart < 10 * 1024 * 1024)
						{
							readsize = idsetdata2[i].idsfilesize - idsetdata2[i].idsfilebufstart;
						}
						else{
							readsize = 10 * 1024 * 1024;
						}
						if (readsize == 0){ batmax = true; break; }
						DWORD readcnt;
						ReadFile(idsetdata2[i].idsfile, idsetdata2[i].idsfilebuf, readsize, &readcnt, 0);
						idsetdata2[i].idsfilebufdatasize = readsize;
						idsetdata2[i].idsfilebufstart += readsize;
						idsetdata2[i].pidsfilebuf = idsetdata2[i].idsfilebuf;
					}
				}

				if (idsize == 4)
				{
					if (idsetdata2[i].pidsfilebuf+4 - idsetdata2[i].idsfilebuf<idsetdata2[i].idsfilebufdatasize){ batmax = true; break; }
					if (((uint32_t*)idsetdata2[i].pidsfilebuf)[0]<id)
					{
						idsetdata2[i].pidsfilebuf += 4;
					}
				}
				else if (idsize == 8)
				{
					if (idsetdata2[i].pidsfilebuf + 8 - idsetdata2[i].idsfilebuf<idsetdata2[i].idsfilebufdatasize){ batmax = true; break; }
					if (((uint64_t*)idsetdata2[i].pidsfilebuf)[0]<id)
					{
						idsetdata2[i].pidsfilebuf += 8;
					}
				}
			}
		}
		if (batmax)break;


		//全部都等就找到,并全加1个
		bool ballissameid = true;
		for (int i = 0; i < idsetdata2.size(); i++)
		{
			if (idsetdata2[i].type == 0)
			{
				if (idsize == 4)
				{
					if (idsetdata2[i].pidsbuf + 4 - idsetdata2[i].idsbuf>idsetdata2[i].idsbufsize){ batmax = true; break; }
					if (((uint32_t*)idsetdata2[i].pidsbuf)[0] != id)
					{
						ballissameid = false;
						break;
					}
				}
				else if (idsize == 8)
				{
					if (idsetdata2[i].pidsbuf + 8 - idsetdata2[i].idsbuf>idsetdata2[i].idsbufsize){ batmax = true; break; }
					if (((uint64_t*)idsetdata2[i].pidsbuf)[0] != id)
					{
						ballissameid = false;
						break;
					}
				}
			}
			else if (idsetdata2[i].type == 1)
			{
				if (idsize == 4)
				{
					if (idsetdata2[i].pidsfilebuf + 4 - idsetdata2[i].idsfilebuf<idsetdata2[i].idsfilebufdatasize){ batmax = true; break; }
					if (((uint32_t*)idsetdata2[i].pidsfilebuf)[0] != id)
					{
						ballissameid = false;
						break;
					}
				}
				else if (idsize == 8)
				{
					if (idsetdata2[i].pidsfilebuf + 8 - idsetdata2[i].idsfilebuf<idsetdata2[i].idsfilebufdatasize){ batmax = true; break; }
					if (((uint64_t*)idsetdata2[i].pidsfilebuf)[0]!=id)
					{
						ballissameid = false;
						break;
					}
				}
			}
		}
		if (batmax)
		{
			break;
		}

		if (ballissameid)
		{
			out_ids->append((char*)&id, idsize);
		}

	}

	for (int i = 0; i < idsetdata2.size(); i++)
	{
		if (idsetdata2[i].type == 1)
		{
			CloseHandle(idsetdata2[i].idsfile);
		}
	}

	return 0;
}




#pragma once


#include <set>
//#include <map>
#include <unordered_map>
//#include <hash_map>
//#include <ext/hash_map>
#include <list>
#include <functional>

#include "mpage.h"

// 提高效率可以使用hash系列
#define xh_page_set		std::set
#define xh_page_map		std::unordered_map
//#define xh_page_map		stdext::hash_map
//#define xh_page_map		__gnu_cxx::hash_map
#define xh_page_list	std::list

namespace xhnet
{
	class CPageList
	{
	public:
		typedef xh_page_list<CMPage*>			page_list;
		typedef page_list::iterator				page_list_it;

		CPageList();
		~CPageList();

		void Push(CMPage* page);
		CMPage* Pop();
		CMPage* Find(std::function<bool(CMPage*)> fun);
		void Clear(bool bdelete=false);
		size_t Size() { return m_pages.size(); }

	public:
		page_list m_pages;

	public:
		CPageList(const CPageList&) = delete;
		CPageList& operator=(const CPageList&) = delete;
	};

	class CPageSet
	{
	public:
		typedef xh_page_set<CMPage*>			page_set;
		typedef page_set::iterator				page_set_it;

		CPageSet();
		~CPageSet();

		void Insert(CMPage* page);
		void Erase(CMPage* page);
		CMPage* Find(std::function<bool(CMPage*)> fun);
		void Clear(bool bdelete = false);
		size_t Size() { return m_pages.size(); }

	public:
		page_set m_pages;

	public:
		CPageSet(const CPageSet&) = delete;
		CPageSet& operator=(const CPageSet&) = delete;
	};

	template<class T>
	class CPagesMap
	{
	public:
		typedef xh_page_map<T, CMPage*>			page_map;
		typedef typename page_map::iterator		page_map_it;

		CPagesMap()
		{
		}
		~CPagesMap()
		{
			Clear();
		}

		void Push(T key, CMPage* page)
		{
			m_pages[key] = page;
		}
		CMPage* Find(T key)
		{
			page_map_it it = m_pages.find(key);
			if (it == m_pages.end())
			{
				return 0;
			}

			return it->second;
		}
		CMPage* Pop(T key)
		{
			page_map_it it = m_pages.find(key);
			if (it == m_pages.end())
			{
				return 0;
			}

			CMPage* page = it->second;
			m_pages.erase(it);
			return page;
		}
		void Clear(bool bdelete=false)
		{
			if (bdelete)
			{
				for (page_map_it it = m_pages.begin(); it != m_pages.end(); ++it)
				{
					if (it->second)
					{
						delete (it->second);
					}
				}
			}

			m_pages.clear();
		}

	public:
		page_map m_pages;

	public:
		CPagesMap(const CPagesMap&) = delete;
		CPagesMap& operator=(const CPagesMap&) = delete;
	};


	class CSameSizeMPageMgr
	{
	public:
		// 当空闲队列的长度大于DEFAULT_START_RECYCLE_SIZE，之后回收的内存全部返回给系统
		enum { DEFAULT_START_RECYCLE_SIZE = 50 };

		CSameSizeMPageMgr(unsigned long block_size, unsigned long align_size, unsigned long recycle_size = DEFAULT_START_RECYCLE_SIZE);
		~CSameSizeMPageMgr(void);

		void* AllocateFromPage(CMPage*& allocfrompage);
		void FreeToPage(void* ptr, CMPage* freetopage);

		void GetMemStat(unsigned long& all, unsigned long& used, unsigned long &free);

		void Set_AllPages(CPageSet* pages)
		{
			m_all_pages = pages;
		}

		unsigned long Get_Block_Size(void)
		{
			return m_block_size;
		}

		unsigned long Get_Align_Size(void)
		{
			return m_align_size;
		}

		CMPage*	Get_Cur_AllocPage(void)
		{
			return m_cur_allocpage;
		}
	private:
		unsigned long		m_block_size;
		unsigned long		m_align_size;
		
		unsigned long		m_recycle_size;

		// 所有的chunk
		CPageSet*			m_all_pages;

		// page大小一样的一个page
		CPageList			m_free_pages;
		CPageSet			m_using_pages;

		CMPage*				m_cur_allocpage;
	public:
		CSameSizeMPageMgr(void) = delete;
		CSameSizeMPageMgr(const CSameSizeMPageMgr&) = delete;
		CSameSizeMPageMgr& operator=(const CSameSizeMPageMgr&) = delete;
	};

	class CMPageMgr
	{
	public:
		struct UsingBlockInfo
		{
			CSameSizeMPageMgr*  ppool;
			CMPage*				ppage;

			UsingBlockInfo(void)
				: ppool(0), ppage(0)
			{
			}

			UsingBlockInfo(CSameSizeMPageMgr* pool, CMPage* page)
				: ppool(pool), ppage(page)
			{
			}
		};

		CMPageMgr(unsigned long recycle_size = CSameSizeMPageMgr::DEFAULT_START_RECYCLE_SIZE);
		~CMPageMgr(void);

		void* Allocate(unsigned long block_size, unsigned long align_size);
		void Free(void* ptr);

		void GetMemStat(unsigned long& all, unsigned long& used, unsigned long &free);

	private:
		unsigned long								m_recycle_size;

		CPageSet									m_all_pages;
		
		// alignedblocksize->mgr
		xh_page_map<unsigned long, CSameSizeMPageMgr*>	m_using_pages;

		//
		xh_page_map<void*, UsingBlockInfo>			m_using_map;

	public:
		CMPageMgr(const CMPageMgr&) = delete;
		CMPageMgr& operator=(const CMPageMgr&) = delete;
	};
};

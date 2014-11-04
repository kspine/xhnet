
#pragma once

namespace xhnet
{
	//
	// ������CMPageά�����ڴ����  
	//                   ���������ڴ����Ҫ��  
	//                             |  
	// ----------------------------------------------------------------------  
	// | �ڴ������� | ά����ָ�� | ����1 | ����2 | ����3 | ...... | ����n |  
	// ----------------------------------------------------------------------  
	// ^                     | ָ��malloc()����ĵ�ַ���  
	// |                     |  
	// -----------------------  
	//
	class CMPage
	{
	public:
		enum { DEFAULT_PAGESIZE = 64 * 1024 };
		enum { DEFAULT_ALIGNSIZE = 1 };
		// ����ά���ڴ�����, ���struct��Ϊ����߿ɶ���  
		struct FreeBlock { FreeBlock* next; };

		static unsigned long cast2uint(void* p)
		{
			return reinterpret_cast<unsigned long>(p);
		}

	public:
		CMPage(unsigned long block_size, unsigned long align_size = DEFAULT_ALIGNSIZE);
		~CMPage(void);

		CMPage(void) = delete;
		CMPage(const CMPage&) = delete;
		CMPage& operator=(const CMPage&) = delete;

		bool Reset(unsigned long block_size, unsigned long align_size = DEFAULT_ALIGNSIZE);
		void Clear(void);

		void* Allocate(void);
		void Free(void* ptr);

		unsigned long Get_AlignSize(void)
		{
			return m_align_size;
		}

		unsigned long Get_PageSize(void)
		{
			return m_page_size;
		}

		unsigned long Get_BlockSize(void)
		{
			return m_block_size;
		}

		unsigned long Get_AllBlockNum(void)
		{
			return m_allblock_num;
		}

		unsigned long Get_FreeBlockNum(void)
		{
			return m_freeblock_num;
		}

		// �ǲ���������
		bool Is_Empty(void)
		{
			return m_freeblock_num == 0;
		}

		// �ǲ���û�ù�
		bool Is_Intact(void)
		{
			return m_freeblock_num == m_allblock_num;
		}

		// ���һ��ָ���Ƿ�����Pool�з����, ���ڷ�ֹ�����ͷ�  
		bool Is_ContainsPointer(void* ptr)
		{
			return cast2uint(ptr) >= cast2uint(m_memory) &&
				cast2uint(ptr) < cast2uint(m_memory) + m_block_size*m_allblock_num;
		}

		static unsigned long Calc_AlignedSize(unsigned long block_size, unsigned long align_size);
		static unsigned long Calc_PageSize(unsigned long block_size, unsigned long align_size, unsigned long aligned_block_size);
		static unsigned long Calc_PageSize(unsigned long block_size, unsigned long align_size);

	private:
		void create(unsigned long block_size, unsigned long align_size);
		void destory(void);

		FreeBlock* allocateblock();
		void freeblock(FreeBlock* block);

		bool isaligned(void* data, long alignment)
		{
			// ����һ�������㷨, �μ�<Hacker's Delight>  
			return (cast2uint(data) & (unsigned long)(alignment - 1)) == 0;
		}

	public:
		// �ڴ����
		void*				m_memory;			// A pointer to the memory allocated for the entire pool                
		FreeBlock*			m_freeblock_head;	// A pointer to the head of the linked list of free blocks  
		unsigned long		m_freeblock_num;	// The current number of free blocks in the pool  

		// �ܴ�С
		unsigned long		m_allblock_num;		// All number of blocks in the pool
		unsigned long		m_page_size;

		// ÿ���ڴ��С�Ͷ��볤��
		unsigned long		m_align_size;
		unsigned long		m_block_size;		// The size in bytes of a block in the pool (this is only  
												// stored so it can be used by Is_ContainsPointer() to   
												// validate deallocation and so could be omitted in a release build) 
	};
};


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

	public:
		CMPage(unsigned int block_size, unsigned int align_size = DEFAULT_ALIGNSIZE);
		~CMPage(void);

		CMPage(void) = delete;
		CMPage(const CMPage&) = delete;
		CMPage& operator=(const CMPage&) = delete;

		bool Reset(unsigned int block_size, unsigned int align_size = DEFAULT_ALIGNSIZE);
		void Clear(void);

		void* Allocate(void);
		void Free(void* ptr);

		unsigned int Get_AlignSize(void)
		{
			return m_align_size;
		}

		unsigned int Get_PageSize(void)
		{
			return m_page_size;
		}

		unsigned int Get_BlockSize(void)
		{
			return m_block_size;
		}

		unsigned int Get_AllBlockNum(void)
		{
			return m_allblock_num;
		}

		unsigned int Get_FreeBlockNum(void)
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
			return (unsigned int)ptr >= (unsigned int)m_memory &&
				(unsigned int)ptr < (unsigned int)m_memory + m_block_size*m_allblock_num;
		}

		static unsigned int Calc_AlignedSize(unsigned int block_size, unsigned int align_size);
		static unsigned int Calc_PageSize(unsigned int block_size, unsigned int align_size, unsigned int aligned_block_size);
		static unsigned int Calc_PageSize(unsigned int block_size, unsigned int align_size);

	private:
		void create(unsigned int block_size, unsigned int align_size);
		void destory(void);

		FreeBlock* allocateblock();
		void freeblock(FreeBlock* block);

		bool isaligned(void* data, int alignment)
		{
			// ����һ�������㷨, �μ�<Hacker's Delight>  
			return ((unsigned int)data & (alignment - 1)) == 0;
		}

	public:
		// �ڴ����
		void*				m_memory;			// A pointer to the memory allocated for the entire pool                
		FreeBlock*			m_freeblock_head;	// A pointer to the head of the linked list of free blocks  
		unsigned int		m_freeblock_num;	// The current number of free blocks in the pool  

		// �ܴ�С
		unsigned int		m_allblock_num;		// All number of blocks in the pool
		unsigned int		m_page_size;

		// ÿ���ڴ��С�Ͷ��볤��
		unsigned int		m_align_size;
		unsigned int		m_block_size;		// The size in bytes of a block in the pool (this is only  
												// stored so it can be used by Is_ContainsPointer() to   
												// validate deallocation and so could be omitted in a release build) 
	};
};

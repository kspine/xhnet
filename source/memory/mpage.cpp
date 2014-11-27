
#include <cassert>
#include <memory>
#include <stdlib.h>
#include <assert.h>

#include "memory/mpage.h"

namespace xhnet
{
	CMPage::CMPage(unsigned long block_size, unsigned long align_size)
		: m_memory(0), m_freeblock_head(0), m_freeblock_num(0)
		, m_allblock_num(0), m_page_size(0)
		, m_align_size(0), m_block_size(0)
	{
		if (block_size < 1)	block_size = 1;
		if (align_size < 1) align_size = 1;

		create(block_size, align_size);
	}

	CMPage::~CMPage(void)
	{
		destory();
	}

	bool CMPage::Reset(unsigned long block_size, unsigned long align_size)
	{
		if (block_size < 0)
			return false;

		if (align_size < 0)
			return false;

		if (!Is_Intact())
			return false;

		if ( m_block_size==block_size && m_align_size==align_size )
		{
			return true;
		}

		create(block_size, align_size);
		return true;
	}

	void CMPage::Clear(void)
	{
		if (!Is_Empty())
		{
			m_freeblock_num = m_allblock_num;

			assert(isaligned(m_block, m_align_size) && "memory not aligned");

			m_freeblock_head = (FreeBlock*)m_block;
			m_freeblock_head->next = 0;
		}
	}

	void* CMPage::Allocate(void)
	{
		// ����ʵ������  
		return allocateblock();
	}

	void CMPage::Free(void* ptr)
	{
		// ���Pool�Ƿ�����Լ����ͷ�ָ���Ƿ�Ϊ��, �Է�ֹ��������  
		if (!ptr) return;

		if (Is_ContainsPointer(ptr) && isaligned(ptr, m_align_size))
		{
			// ���ͷŵ��ڴ滹��Pool  
			freeblock((FreeBlock*)ptr);
		}
		else
		{
			assert(false && "object not allocated from this pool");
		}
	}

	unsigned long CMPage::Calc_AlignedSize(unsigned long block_size, unsigned long align_size)
	{
		// �ڴ��Ĵ�СҪ�����ڴ�����Ҫ��, ��������ʹCPUѰַ���.  
		// __alignof(T)��Ϊ�˼��T���������, ��Ϊ�û�����ָ�����������,  
		// ���Բ�����Hard Code, �����ڵ�STL�ڴ���������, �������ȹ̶�Ϊ8.  
		// ��������û�ָ������Ϊ4, ��ô�ͻ���ִ���, ����Ķ�̬���������  
		//  
		// �ٸ�����, ����sizeof(T) = 11, __alignof(T) = 4  
		// ��ô blockSize = 11  
		//      diff = 11 % 4 = 3  
		// ��Ϊ diff != 0, ����  
		//      blockSize += 11 + 4 - 3 = 12  
		// �����������ڴ�����Ҫ����, �ܲ�����㷨  
		//  
		// ������һ������, ���sizeof(T)��һ��ָ��ҪС, ��ô���˷��ڴ�  
		unsigned long aligned_blocksize = block_size;
		unsigned long diff = aligned_blocksize % align_size;
		if (diff != 0) aligned_blocksize += align_size - diff;

		// ע��: ��������blockSize��һ��ָ�뻹С, ��ô������Ҫ����һ��ָ��Ĵ�С  
		if (aligned_blocksize < sizeof(FreeBlock*)) aligned_blocksize = sizeof(FreeBlock*);

		return aligned_blocksize;
	}

	unsigned long CMPage::Calc_PageSize(unsigned long block_size, unsigned long align_size)
	{
		unsigned long aligned_block_size = Calc_AlignedSize(block_size, align_size);
		unsigned long maxfilledsize = align_size - 1;

		unsigned long pagesize = DEFAULT_PAGESIZE;

		if (aligned_block_size + maxfilledsize > DEFAULT_PAGESIZE)
		{
			pagesize = aligned_block_size + maxfilledsize;
		}

		return pagesize;
	}

	void CMPage::create(unsigned long block_size, unsigned long align_size)
	{
		unsigned long src_pagesize = Get_PageSize();

		// �õ�������һ��block���ڴ��С��ÿ��block�������sizeof(FreeBlock*)
		m_block_size	= Calc_AlignedSize(block_size, align_size);
		m_align_size	= align_size;

		// ������ά��FreeBlockָ��ռ�õ��ڴ��С  
		//unsigned long pointersize	= sizeof(FreeBlock*);
		unsigned long maxfilledsize = m_align_size - 1;

		if (m_block_size + maxfilledsize > DEFAULT_PAGESIZE)
		{
			m_page_size		= m_block_size + maxfilledsize;
			m_allblock_num	= 1;
		}
		else
		{
			m_page_size		= DEFAULT_PAGESIZE;
			m_allblock_num	= (m_page_size - maxfilledsize) / m_block_size;
		}

		m_freeblock_num = m_allblock_num;

		// ���ԭ���ĸ����ڵ��ڴ�һ����С �Ͳ��ٷ���
		if (m_memory || src_pagesize != m_page_size)
		{
			if (m_memory)
			{
				::free(m_memory);
			}

			m_memory = ::malloc(m_page_size);
		}

		m_block = getaligned(m_memory, m_align_size);

		// �������ڴ��Ƿ������ڴ��������,
		assert(isaligned(m_block, m_align_size) && "memory not aligned");

		// ��FreeBlock����ͷ����Ϊ�����ֵ  
		m_freeblock_head = (FreeBlock*)m_block;
		m_freeblock_head->next = 0;
	}

	void CMPage::destory(void)
	{
		// �ͷŲ����ܼ���, �μ���ͼ  
		if (m_memory)
		{
			::free(m_memory);
			m_memory = 0;
		}
	}

	CMPage::FreeBlock* CMPage::allocateblock()
	{
		if (m_freeblock_num == 0)
		{
			return 0;
		}

		// ����block��һ��O(1)���㷨������ͷʼ���ǿ��нڵ�  
		FreeBlock* block = m_freeblock_head;

		// ����ά�����ǿ��нڵ����Ŀ, ��Pool��ʣ������  
		if (--m_freeblock_num != 0)
		{
			if (m_freeblock_head->next == NULL)
			{
				// If the block has not been previously allocated its next pointer  
				// will be NULL so just update the list head to the next block in the pool   
				m_freeblock_head = (FreeBlock*)(cast2uint(m_freeblock_head) + m_block_size);
				m_freeblock_head->next = NULL;
			}
			else
			{
				// The block has been previously allocated and freed so it   
				// has a valid link to the next free block  
				m_freeblock_head = m_freeblock_head->next;
			}
		}
		else
		{
			m_freeblock_head = 0;
		}

		return block;
	}

	void CMPage::freeblock(FreeBlock* block)
	{
		// ���ڴ�黹������ͷ  
		if (m_freeblock_num > 0) block->next = m_freeblock_head;
		m_freeblock_head = block;

		// ά�����нڵ���Ŀ  
		m_freeblock_num++;
	}
};


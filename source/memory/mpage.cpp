
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
		create(block_size, align_size);
	}

	CMPage::~CMPage(void)
	{
		destory();
	}

	bool CMPage::Reset(unsigned long block_size, unsigned long align_size)
	{
		if (!Is_Intact())
		{
			return false;
		}

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

			assert(isaligned(m_memory, m_align_size) && "memory not aligned");

			m_freeblock_head = (FreeBlock*)m_memory;
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

	unsigned long CMPage::Calc_PageSize(unsigned long block_size, unsigned long align_size, unsigned long aligned_block_size)
	{
		unsigned long pointersize = sizeof(FreeBlock*);
		unsigned long moresize = aligned_block_size - block_size + pointersize;

		unsigned long pagesize = 0;
		if (aligned_block_size + moresize > DEFAULT_PAGESIZE)
		{
			pagesize = aligned_block_size + moresize;
		}
		else
		{
			pagesize = DEFAULT_PAGESIZE;
		}

		return pagesize;
	}

	unsigned long CMPage::Calc_PageSize(unsigned long block_size, unsigned long align_size)
	{
		return Calc_PageSize(block_size, align_size, Calc_AlignedSize(block_size, align_size));
	}

	void CMPage::create(unsigned long block_size, unsigned long align_size)
	{
		// �õ�������һ��block���ڴ��С��ÿ��block�������sizeof(FreeBlock*)
		m_block_size = Calc_AlignedSize(block_size, align_size);

		// �����㹻���ڴ�, ������㷨�ܾ���, ���ڵ�STL��ʹ�õľ�������㷨  
		// ������ά��FreeBlockָ��ռ�õ��ڴ��С  
		unsigned long pointersize = sizeof(FreeBlock*);
		unsigned long moresize = m_block_size - block_size + pointersize;

		if (m_block_size + moresize > DEFAULT_PAGESIZE)
		{
			m_align_size	= align_size;
			m_page_size		= m_block_size + moresize;
			m_allblock_num	= 1;
			m_freeblock_num = m_allblock_num;
		}
		else
		{
			m_align_size	= align_size;
			m_page_size		= DEFAULT_PAGESIZE;
			m_allblock_num	= (m_page_size - moresize) / m_block_size;
			m_freeblock_num = m_allblock_num;
		}

		// ���ԭ���ĸ����ڵ��ڴ�һ����С �Ͳ��ٷ���
		unsigned long src_pagesize = Get_PageSize();
		void* src_raw = 0;
		if (m_memory)
		{
			src_raw = *(void**)(cast2uint(m_memory) - pointersize);
		}
		void* raw = 0;
		if (src_raw)
		{
			if (src_pagesize == m_page_size)
			{
				raw = src_raw;
			}
			else
			{
				::free(src_raw);
				raw = ::malloc(m_page_size);
			}
		}
		else
		{
			// �����ʵ�ʴ�С����20000 + 7 = 20007  
			raw = ::malloc(m_page_size);
		}

		// ����ʵPool����Ϊ����ʵ��������ڴ��ַ  
		unsigned long start = cast2uint(raw) + moresize;
		void* aligned = (void*)((start + m_align_size - 1)&(~(m_align_size - 1)));

		// ����ά��һ��ָ��malloc()����������ڴ�  
		*(void**)(cast2uint(aligned) - pointersize) = raw;

		m_memory = aligned;

		// �������ڴ��Ƿ������ڴ��������, �������˸о�û��Ҫ���м��  
		assert(isaligned(m_memory, m_align_size) && "memory not aligned");

		// ��FreeBlock����ͷ����Ϊ�����ֵ  
		m_freeblock_head = (FreeBlock*)m_memory;
		m_freeblock_head->next = 0;
	}

	void CMPage::destory(void)
	{
		// �ͷŲ����ܼ���, �μ���ͼ  
		void* raw = *(void**)(cast2uint(m_memory) - sizeof(void*));
		::free(raw);
		m_memory = 0;
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


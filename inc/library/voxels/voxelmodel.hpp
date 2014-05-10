#ifndef VOXELMODEL_HPP
#define VOXELMODEL_HPP

#include <library/opengl/vao.hpp>

/**
 * Create, check that its "good", then render!
 * This model is specially made to be used with the XModel class
 * 
**/

namespace library
{
	class VoxelModel
	{
	private:
		VAO vao;
		
	public:
		VoxelModel() {}
		VoxelModel(const VoxelModel& vx);
		
		void create(GLsizei vertices, GLvoid* data);
		
		inline bool isGood() const
		{
			return vao.isGood();
		}
		
		void render();
		void render(GLint mode);
	};
}

#endif

#include "meshloader.h"
#include <map>
#include <queue>
#include <memory>
#include <algorithm>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

std::vector<MVertex> MeshTxtGen::vertices;
std::vector<MeshData> MeshTxtGen::mesh_datas;
std::vector<std::string> MeshTxtGen::mt_files;

bool MeshTxtGen::gen_mesh_txt(std::string& res, MeshTxtGenType type, uint depth, SourceType source_type) {
	std::ostream* out;
	std::ofstream fs;
	std::stringstream ss;
	if (source_type == SourceType::BY_FILE) {
		fs.open("resources/models/txt/" + res, std::ios::trunc | std::ios::out);
		if (!fs.is_open()) { return false; }
		out = &fs;
	}
	else if (source_type == SourceType::BY_STRING) { ss.clear(); out = &ss; }
    else { return false; }

	out->fill('0');
	out->precision(6);
	out->setf(std::ios::fixed, std::ios::floatfield);

	vertices.clear();
	mesh_datas.clear();
	mt_files.clear();

	bool has_normal = false;
	switch (type) {
	case CIRCLE:			gen_circle(depth); break;
	case RECTANGLE:			gen_rect(); break;
	case TRIANGLE_RIGHT:	gen_triangle_right(); break;
	case TRIANGLE_REGULAR:	gen_triangle_regular(); break;
	case CUBE:				gen_cube(); break;
	case CONE:				gen_cone(depth); break;
	case SPHERE:			gen_sphere(depth); has_normal = true; break;
	case CYLINDER:			gen_cylinder(depth); break;
	}

	if(!has_normal) cac_normal();

	write((*out));
	if (source_type == SourceType::BY_FILE)		{ fs.close(); }
	else if (source_type == SourceType::BY_STRING) { res = ss.str(); ss.clear(); }

	return true;
}

void MeshTxtGen::gen_triangle_right() {
	MVertex a(CVector3D(-0.5f, -0.5f, 0.0f));
	MVertex b(CVector3D( 0.5f, -0.5f, 0.0f));
	MVertex c(CVector3D(-0.5f,  0.5f, 0.0f));

	a.tex_coord = a.position.xy() + CVector2D(0.5f);
	b.tex_coord = b.position.xy() + CVector2D(0.5f);
	c.tex_coord = c.position.xy() + CVector2D(0.5f);

	add_one_vertex(a);
	add_one_vertex(b);
	add_one_vertex(c);

	add_one_face(0, 1, 2);

}
void MeshTxtGen::gen_triangle_regular() {
	float h = std::sin(CMath_ins().deg_to_rad(60.0f));
	MVertex a(CVector3D(-0.5f, -0.5f, 0.0f));
	MVertex b(CVector3D(0.5f, -0.5f, 0.0f));
	MVertex c(CVector3D(0.0f, -0.5f + h, 0.0f));

	a.tex_coord = a.position.xy() + CVector2D(0.5f);
	b.tex_coord = b.position.xy() + CVector2D(0.5f);
	c.tex_coord = c.position.xy() + CVector2D(0.5f);

	add_one_vertex(a);
	add_one_vertex(b);
	add_one_vertex(c);

	add_one_face(0, 1, 2);
}
void MeshTxtGen::gen_rect() {
	MVertex a(CVector3D(-1.0f, -1.0f, 0.0f));
	MVertex b(CVector3D(1.0f, -1.0f, 0.0f));
	MVertex c(CVector3D(1.0f, 1.0f, 0.0f));
	MVertex d(CVector3D(-1.0f, 1.0f, 0.0f));

	a.tex_coord = a.position.xy() / 2.0f + CVector2D(0.5f);
	b.tex_coord = b.position.xy() / 2.0f + CVector2D(0.5f);
	c.tex_coord = c.position.xy() / 2.0f + CVector2D(0.5f);
	d.tex_coord = d.position.xy() / 2.0f + CVector2D(0.5f);

	add_one_vertex(a);
	add_one_vertex(b);
	add_one_vertex(c);
	add_one_vertex(d);

	add_one_face(0, 1, 2);
	add_one_face(0, 2, 3);
}
void MeshTxtGen::gen_circle(uint depth) {
	MVertex o(CVector3D(0.0f, 0.0f, 0.0f), CVector3D(), CVector2D(0.5f));
	add_one_vertex(o);
	float rad = 2.0f*CMath_ins().pi / depth;
	for (uint i = 0; i < depth; ++i) {
		MVertex t(CVector3D(0.5f*std::cos(i*rad), 0.5f*std::sin(i*rad), 0.0f));
		t.tex_coord = t.position.xy() + CVector2D(0.5f);
		add_one_vertex(t);
	}
	for (uint i = 1; i <= depth; ++i) { add_one_face(0, i, i%depth + 1); }
}

void MeshTxtGen::gen_cube() {
	// front
	{
		MVertex a(CVector3D(-0.5f, -0.5f, -0.5f));
		MVertex b(CVector3D(0.5f, -0.5f, -0.5f));
		MVertex c(CVector3D(0.5f, 0.5f, -0.5f));
		MVertex d(CVector3D(-0.5f, 0.5f, -0.5f));

		a.tex_coord = a.position.xy() + CVector2D(0.5f);
		b.tex_coord = b.position.xy() + CVector2D(0.5f);
		c.tex_coord = c.position.xy() + CVector2D(0.5f);
		d.tex_coord = d.position.xy() + CVector2D(0.5f);

		add_one_vertex(a);	// 0
		add_one_vertex(b);	// 1
		add_one_vertex(c);	// 2
		add_one_vertex(d);	// 3

		add_one_face(0, 1, 2);
		add_one_face(0, 2, 3);
	}

	// back
	{
		MVertex a(CVector3D(0.5f, -0.5f, 0.5f), CVector3D(), CVector2D(0.0f, 0.0f));
		MVertex b(CVector3D(-0.5f, -0.5f, 0.5f), CVector3D(), CVector2D(1.0f, 0.0f));
		MVertex c(CVector3D(-0.5f, 0.5f, 0.5f), CVector3D(), CVector2D(1.0f, 1.0f));
		MVertex d(CVector3D(0.5f, 0.5f, 0.5f), CVector3D(), CVector2D(0.0f, 1.0f));

		add_one_vertex(a);	// 4
		add_one_vertex(b);	// 5
		add_one_vertex(c);	// 6
		add_one_vertex(d);	// 7

		add_one_face(4, 5, 6);
		add_one_face(4, 6, 7);
	}

	// right
	{
		MVertex a(CVector3D(0.5f, -0.5f, -0.5f), CVector3D(), CVector2D(0.0f, 0.0f));
		MVertex b(CVector3D(0.5f, -0.5f, 0.5f), CVector3D(), CVector2D(1.0f, 0.0f));
		MVertex c(CVector3D(0.5f, 0.5f, 0.5f), CVector3D(), CVector2D(1.0f, 1.0f));
		MVertex d(CVector3D(0.5f, 0.5f, -0.5f), CVector3D(), CVector2D(0.0f, 1.0f));

		add_one_vertex(a);	// 8
		add_one_vertex(b);	// 9
		add_one_vertex(c);	// 10
		add_one_vertex(d);	// 11

		add_one_face(8, 9, 10);
		add_one_face(8, 10, 11);
	}
	
	// left
	{
		MVertex a(CVector3D(-0.5f, -0.5f, 0.5f), CVector3D(), CVector2D(0.0f, 0.0f));
		MVertex b(CVector3D(-0.5f, -0.5f, -0.5f), CVector3D(), CVector2D(1.0f, 0.0f));
		MVertex c(CVector3D(-0.5f, 0.5f, -0.5f), CVector3D(), CVector2D(1.0f, 1.0f));
		MVertex d(CVector3D(-0.5f, 0.5f, 0.5f), CVector3D(), CVector2D(0.0f, 1.0f));

		add_one_vertex(a);	// 12
		add_one_vertex(b);	// 13
		add_one_vertex(c);	// 14
		add_one_vertex(d);	// 15

		add_one_face(12, 13, 14);
		add_one_face(12, 14, 15);
	}

	// top
	{
		MVertex a(CVector3D(-0.5f, 0.5f, -0.5f), CVector3D(), CVector2D(0.0f, 0.0f));
		MVertex b(CVector3D(0.5f, 0.5f, -0.5f), CVector3D(), CVector2D(1.0f, 0.0f));
		MVertex c(CVector3D(0.5f, 0.5f, 0.5f), CVector3D(), CVector2D(1.0f, 1.0f));
		MVertex d(CVector3D(-0.5f, 0.5f, 0.5f), CVector3D(), CVector2D(0.0f, 1.0f));

		add_one_vertex(a);	// 16
		add_one_vertex(b);	// 17
		add_one_vertex(c);	// 18
		add_one_vertex(d);	// 19

		add_one_face(16, 17, 18);
		add_one_face(16, 18, 19);
	}

	// bottom
	{
		MVertex a(CVector3D(-0.5f, -0.5f, 0.5f), CVector3D(), CVector2D(0.0f, 0.0f));
		MVertex b(CVector3D(0.5f, -0.5f, 0.5f), CVector3D(), CVector2D(1.0f, 0.0f));
		MVertex c(CVector3D(0.5f, -0.5f, -0.5f), CVector3D(), CVector2D(1.0f, 1.0f));
		MVertex d(CVector3D(-0.5f, -0.5f, -0.5f), CVector3D(), CVector2D(0.0f, 1.0f));

		add_one_vertex(a);	// 20
		add_one_vertex(b);	// 21
		add_one_vertex(c);	// 22
		add_one_vertex(d);	// 23

		add_one_face(20, 21, 22);
		add_one_face(20, 22, 23);
	}
	
}
void MeshTxtGen::gen_cone(uint depth) {
	// bottom
	{
		MVertex o(CVector3D(0.0f, -0.5f, 0.0f), CVector3D(), CVector2D(0.5f));
		add_one_vertex(o);
		float rad = 2.0f*CMath_ins().pi / depth;
		for (uint i = 0; i < depth; ++i) {
			MVertex t(CVector3D(0.5f*std::cos(i*rad), -0.5f, 0.5f*std::sin(i*rad)));
			t.tex_coord = t.position.xz() + CVector2D(0.5f);
			add_one_vertex(t);
		}
		for (uint i = 1; i <= depth; ++i) { add_one_face(0, i%depth + 1, i); }
	}

	// side
	{
		MVertex o(CVector3D(0.0f, 0.5f, 0.0f), CVector3D(), CVector2D(0.5f));
		add_one_vertex(o);
		float rad = 2.0f*CMath_ins().pi / depth;
		for (uint i = 0; i < depth; ++i) {
			MVertex t(CVector3D(0.5f*std::cos(i*rad), -0.5f, 0.5f*std::sin(i*rad)));
			t.tex_coord = t.position.xz() + CVector2D(0.5f);
			add_one_vertex(t);
		}
		for (uint i = 1; i <= depth; ++i) { add_one_face(depth + 1, i + depth + 1, i%depth + 1 + depth + 1); }
	}
}
void MeshTxtGen::gen_sphere(uint depth) {
	depth = CMath_ins().clamp<uint>(depth, 1, 5);

	// first depth
	{
		--depth;

		MVertex o(CVector3D(0.0f, 1.0f, 0.0f));		//  y
		MVertex a(CVector3D(1.0f, 0.0f, 0.0f));		//  x
		MVertex b(CVector3D(0.0f, 0.0f, -1.0f));	// -z
		MVertex c(CVector3D(-1.0f, 0.0f, 0.0f));	// -x
		MVertex d(CVector3D(0.0f, 0.0f, 1.0f));		//  z
		MVertex e(CVector3D(1.0f, 0.0f, 0.0f));		//  x 
		MVertex f(CVector3D(0.0f, -1.0f, 0.0f));	// -y

		o.tex_coord = CVector2D(0.00f, 1.0f);
		a.tex_coord = CVector2D(0.00f, 0.5f);
		b.tex_coord = CVector2D(0.25f, 0.5f);
		c.tex_coord = CVector2D(0.50f, 0.5f);
		d.tex_coord = CVector2D(0.75f, 0.5f);
		e.tex_coord = CVector2D(1.00f, 0.5f);
		f.tex_coord = CVector2D(0.00f, 0.0f);

		add_one_vertex(o);	// 0
		add_one_vertex(a);	// 1
		add_one_vertex(b);	// 2
		add_one_vertex(c);	// 3
		add_one_vertex(d);	// 4
		add_one_vertex(e);	// 5
		add_one_vertex(f);	// 6
	
		add_one_face(0, 1, 2);	//  y  x -z
		add_one_face(0, 2, 3);	//  y -z -x
		add_one_face(0, 3, 4);	//  y -x  z
		add_one_face(0, 4, 5);	//  y  z  x 

		add_one_face(6, 5, 4);	// -y  x  z   
		add_one_face(6, 4, 3);	// -y  z -x
		add_one_face(6, 3, 2);	// -y -x -z
		add_one_face(6, 2, 1);	// -y -z  x

	}

	// loop depth
	{
		std::map<std::pair<uint, uint>, uint> v_mid;
		uint cur_id = 7;
		while (depth--) {
			for (auto& md : mesh_datas) {
				auto& indices = md.indices;
                uint len = static_cast<uint>(indices.size());
				for (uint i = 0; i < len; i += 3) {
					uint i_a = indices[i + 0];
					uint i_b = indices[i + 1];
					uint i_c = indices[i + 2];
					uint i_d, i_e, i_f;

					auto a = vertices[i_a];
					auto b = vertices[i_b];
					auto c = vertices[i_c];

					auto puu_ab = std::make_pair(std::min(i_a, i_b), std::max(i_a, i_b));
					auto puu_bc = std::make_pair(std::min(i_b, i_c), std::max(i_b, i_c));
					auto puu_ca = std::make_pair(std::min(i_c, i_a), std::max(i_c, i_a));

					if (!v_mid.count(puu_ab)) {
						MVertex d = MVertex((a.position + b.position).normalize());
						d.tex_coord = (a.tex_coord + b.tex_coord) * 0.5f;
                        if (CMath_ins().fcmp(a.position.y(), 1.0f) == 0 || CMath_ins().fcmp(a.position.y(), -1.0f) == 0) {
							d.tex_coord.set_x(b.tex_coord.x());
                        }
                        if (CMath_ins().fcmp(b.position.y(), 1.0f) == 0 || CMath_ins().fcmp(b.position.y(), -1.0f) == 0) {
							d.tex_coord.set_x(a.tex_coord.x());
                        }
						add_one_vertex(d);
						i_d = cur_id++; v_mid[puu_ab] = i_d;
					}
					else { i_d = v_mid[puu_ab]; }

					if (!v_mid.count(puu_bc)) {
						MVertex e = MVertex((b.position + c.position).normalize());
						e.tex_coord = (b.tex_coord + c.tex_coord) * 0.5f;
                        if (CMath_ins().fcmp(b.position.y(), 1.0f) == 0 || CMath_ins().fcmp(b.position.y(), -1.0f) == 0) {
                            e.tex_coord.set_x(c.tex_coord.x());
                        }
                        if (CMath_ins().fcmp(c.position.y(), 1.0f) == 0 || CMath_ins().fcmp(c.position.y(), -1.0f) == 0) {
                            e.tex_coord.set_x(b.tex_coord.x());
                        }
						add_one_vertex(e);
						i_e = cur_id++; v_mid[puu_bc] = i_e;
					}
					else { i_e = v_mid[puu_bc]; }

					if (!v_mid.count(puu_ca)) {
						MVertex f = MVertex((c.position + a.position).normalize());
						f.tex_coord = (c.tex_coord + a.tex_coord) * 0.5f;
                        if (CMath_ins().fcmp(c.position.y(), 1.0f) == 0 || CMath_ins().fcmp(c.position.y(), -1.0f) == 0) {
                            f.tex_coord.set_x(a.tex_coord.x());
                        }
                        if (CMath_ins().fcmp(a.position.y(), 1.0f) == 0 || CMath_ins().fcmp(a.position.y(), -1.0f) == 0) {
                            f.tex_coord.set_x(c.tex_coord.x());
                        }
						add_one_vertex(f);
						i_f = cur_id++; v_mid[puu_ca] = i_f;
					}
					else { i_f = v_mid[puu_ca]; }

					add_one_face(i_a, i_d, i_f);
					add_one_face(i_d, i_b, i_e);
					add_one_face(i_f, i_e, i_c);
					add_one_face(i_d, i_e, i_f);
				}
				indices.erase(indices.begin(), indices.begin() + len);
			}
			
		}

	}

	for (auto& i : vertices) { i.tex_coord = cac_sphere_tex_coord(i.position.get_normalize()); }

	for (auto& i : vertices) { i.normal = i.position.get_normalize(); }
}
void MeshTxtGen::gen_cylinder(uint depth) {
	add_one_mtfile("resources/materials/txt/single_material.txt");

	// top
	{
		MVertex o(CVector3D(0.0f, 0.5f, 0.0f), CVector3D(), CVector2D(0.5f));
		add_one_vertex(o);
		float rad = 2.0f*CMath_ins().pi / depth;
		for (uint i = 0; i < depth; ++i) {
			MVertex t(CVector3D(0.5f*std::cos(i*rad), 0.5f, 0.5f*std::sin(i*rad)));
			t.tex_coord = t.position.xz() + CVector2D(0.5f);
			add_one_vertex(t);
		}

		add_one_material("emerald");
		for (uint i = 1; i <= depth; ++i) { add_one_face(0, i%depth + 1, i); }
	}
	
	uint cnt = depth + 1;
	// bottom
	{
		MVertex o(CVector3D(0.0f, -0.5f, 0.0f), CVector3D(), CVector2D(0.5f));
		add_one_vertex(o);
		float rad = 2.0f*CMath_ins().pi / depth;
		for (uint i = 0; i < depth; ++i) {
			MVertex t(CVector3D(0.5f*std::cos(i*rad), -0.5f, 0.5f*std::sin(i*rad)));
			t.tex_coord = t.position.xz() + CVector2D(0.5f);
			add_one_vertex(t);
		}

		add_one_material("red_plastic");
		for (uint i = 1; i <= depth; ++i) { add_one_face(cnt, cnt + i, cnt + i%depth + 1); }
	}

	cnt += depth + 1;
	// side
	{
		float rad = 2.0f*CMath_ins().pi / depth;
		for (uint i = 0; i <= depth; ++i) {
			MVertex t(CVector3D(0.5f*std::cos(i*rad), -0.5f, 0.5f*std::sin(i*rad)));
            t.tex_coord = CVector2D(1.0f * i / depth, 0.0f);
			add_one_vertex(t);
			MVertex tt(CVector3D(0.5f*std::cos(i*rad), 0.5f, 0.5f*std::sin(i*rad)));
            tt.tex_coord = CVector2D(1.0f * i / depth, 1.0f);
			add_one_vertex(tt);
		}

		add_one_material("cyan_plastic");
		for (uint i = 0; i < depth; ++i) {
			add_one_face(cnt + i*2, cnt + i*2 + 3, cnt + i*2 + 2);
			add_one_face(cnt + i*2, cnt + i*2 + 1, cnt + i*2 + 3);
		}
	}
}

void MeshTxtGen::write(std::ostream& out) {
	// mt files
	for (auto i : mt_files) { write_one_mtfile(out, i); }
	out << std::endl;

	// vertices
	for (auto i : vertices) { write_one_vertex(out, i);  }
	out << std::endl <<std::endl;

	// materials and indices
	for (auto i : mesh_datas) {
		if (i.material.name.compare("") != 0) { write_one_material(out, i.material.name); }
		for (uint j = 0; j < i.indices.size(); j += 3) {
			write_one_face(out, i.indices[j + 0], i.indices[j + 1], i.indices[j + 2]);
		}
		out << std::endl;
	}
}

void MeshTxtGen::write_one_vertex(std::ostream& out, const MVertex& vertex) {
	out << "v ";
	if (vertex.position.x() >=0.0f) out << " "; out << vertex.position.x() << ", ";
	if (vertex.position.y() >= 0.0f) out << " "; out << vertex.position.y() << ", ";
	if (vertex.position.z() >= 0.0f) out << " "; out << vertex.position.z() << ", ";

	if (vertex.normal.x() >= 0.0f) out << " "; out << vertex.normal.x() << ", ";
	if (vertex.normal.y() >= 0.0f) out << " "; out << vertex.normal.y() << ", ";
	if (vertex.normal.z() >= 0.0f) out << " "; out << vertex.normal.z() << ", ";
	
	if (vertex.tex_coord.x() >= 0.0f) out << " "; out << vertex.tex_coord.x() << ", ";
	if (vertex.tex_coord.y() >= 0.0f) out << " "; out << vertex.tex_coord.y() << ", ";
	out << std::endl;
}
void MeshTxtGen::write_one_face(std::ostream& out, uint a, uint b, uint c) {
	out << "f ";
	out << a << ", "; out << b << ", "; out << c << ", "; 
	out << std::endl;
}
void MeshTxtGen::write_one_material(std::ostream& out, const std::string& mt) {
	out << "use_mt " << mt << std::endl;
}
void MeshTxtGen::write_one_mtfile(std::ostream& out, const std::string& path) {
	out << "mt_file " << path << std::endl;
}

void MeshTxtGen::add_one_vertex(const MVertex& x) { vertices.push_back(x); }
void MeshTxtGen::add_one_face(uint a, uint b, uint c) {
	// 利用混合积保持三角面片的一致性顺序 -- 逆时针
	//float det = vertices[a].position.mix(vertices[b].position, vertices[c].position);
    //if (CMath_ins().fcmp(det, 0.0f) < 0) { std::swap(b, c); }

	if (mesh_datas.size() == 0) mesh_datas.push_back(MeshData());
	mesh_datas[mesh_datas.size()-1].indices.push_back(a); 
	mesh_datas[mesh_datas.size()-1].indices.push_back(b); 
	mesh_datas[mesh_datas.size()-1].indices.push_back(c);
}
void MeshTxtGen::add_one_material(const std::string& mt) {
	mesh_datas.push_back(MeshData());
	mesh_datas[mesh_datas.size() - 1].material = mt;
}
void MeshTxtGen::add_one_mtfile(const std::string& path) {
	mt_files.push_back(path);
}

void MeshTxtGen::cac_normal() {
	for (auto& i : vertices) { i.normal = CVector3D(); }
	for (auto md : mesh_datas) {
		auto indices = md.indices;
		for (uint i = 0; i < indices.size(); i += 3) {
			auto& a = vertices[indices[i + 0]];
			auto& b = vertices[indices[i + 1]];
			auto& c = vertices[indices[i + 2]];

			// cac face normal
			CVector3D t_normal = (b.position - a.position).cross(c.position - a.position);	// 右手定则

			// accumulate normal
			a.normal += t_normal;
			b.normal += t_normal;
			c.normal += t_normal;
		}
	}
	
	// normalize normal
	for (auto& i : vertices) { i.normal.normalize(); }
}
CVector2D MeshTxtGen::cac_sphere_tex_coord(CVector3D v) {
	CVector2D res = CVector2D(std::atan2f(v.z(), v.x()), std::asinf(v.y()));

	res = res / CVector2D(2.0f * CMath_ins().pi, CMath_ins().pi) + 0.5;

	return res;
}

// ===============================================================================================

MVertexBone::MVertexBone() { for (int i = 0; i < m_bone_num_per_vertex; ++i) { ids[i] = 0; weights[i] = 0.0f; } }
MVertexBone& MVertexBone::add(uint id, float weight) {
	for (int i = 0; i < m_bone_num_per_vertex; ++i) {
		if (CMath_ins().fcmp(weights[i], 0.0f) == 0) { ids[i] = id; weights[i] = weight; break; }
	}
	return (*this);
}

/*
CMatrix4x4 aimat_to_cmat(aiMatrix4x4 m) {
	return CMatrix4x4(
		m.a1, m.b1, m.c1, m.d1,
		m.a2, m.b2, m.c2, m.d2,
		m.a3, m.b3, m.c3, m.d3,
		m.a4, m.b4, m.c4, m.d4);
}
CMatrix4x4 aimat_to_cmat(aiMatrix3x3 m) {
	return CMatrix4x4(
		m.a1, m.b1, m.c1, 0.0f,
		m.a2, m.b2, m.c2, 0.0f,
		m.a3, m.b3, m.c3, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}
*/

CMatrix4x4 aimat_to_cmat(aiMatrix4x4 m) {
	return CMatrix4x4(
		m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4);
}
CMatrix4x4 aimat_to_cmat(aiMatrix3x3 m) {
	return CMatrix4x4(
		m.a1, m.a2, m.a3, 0.0f,
		m.b1, m.b2, m.b3, 0.0f,
		m.c1, m.c2, m.c3, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

// ======================================================================

bool MeshLoader::load_mesh_txt(const std::string& src, std::vector<MeshData>& mds, std::vector<std::string>& mt_files, SourceType source_type) {

	// 打开文件
	std::istream* in;
	std::ifstream fs;
	std::istringstream ss;
	if (source_type == SourceType::BY_FILE) {
		fs.open(src, std::ios::in);
		if (!fs.is_open()) { return false; }
		in = &fs;
	}
	else if (source_type == SourceType::BY_STRING) { ss.clear(); ss.str(src); in = &ss; }
    else { return false; }

	std::vector<MVertex> vertices;
	vertices.clear();
	mds.clear();
	mt_files.clear();

	// 按行读取 -- 获得 vertices 和 indices
	while ((*in)) {
		std::string t_line;
		std::getline((*in), t_line);
		std::replace(t_line.begin(), t_line.end(), ',', ' ');
		std::istringstream t_iss(t_line);

		std::string head; t_iss >> head;

		if (head.compare("v") == 0) {
			std::vector<float> list; float t_f;
			while (t_iss >> t_f) list.push_back(t_f);

			MVertex v;
			if (list.size() >= 3) v.position = CVector3D(list[0], list[1], list[2]);
			if (list.size() >= 6) v.normal = CVector3D(list[3], list[4], list[5]);
			if (list.size() >= 8) v.tex_coord = CVector2D(list[6], list[7]);
			vertices.push_back(v);
		}
		else if (head.compare("f") == 0) { 
			if (mds.size() == 0) {
				mds.push_back(MeshData());
				mds.back().vertices.assign(vertices.begin(), vertices.end());
			}
			uint t_u; while (t_iss >> t_u) mds.back().indices.push_back(t_u); 
		}
		else if (head.compare("mt_file") == 0) {
			std::string t_path; t_iss >> t_path;
			mt_files.push_back(t_path);
		}
		else if (head.compare("use_mt") == 0) {
			std::string t_mt; t_iss >> t_mt;
			mds.push_back(MeshData());
			mds.back().material.name = t_mt;
			mds.back().vertices.assign(vertices.begin(), vertices.end());
		}

	}

	// 生成切线
	for (auto& t_md : mds) { gen_tangent(t_md); }

	if (source_type == SourceType::BY_FILE) { fs.close(); }
	
	return true;
}

bool MeshLoader::load_mesh_obj(const std::string& src, std::vector<MeshData>& mds, std::vector<std::string>& mt_files, SourceType source_type) {
	// 打开文件
	std::istream* in;
	std::ifstream fs;
	std::istringstream ss;
	if (source_type == SourceType::BY_FILE) {
		fs.open(src, std::ios::in);
		if (!fs.is_open()) { return false; }
		in = &fs;
	}
	else if (source_type == SourceType::BY_STRING) { ss.clear(); ss.str(src); in = &ss; }
    else { return false; }

	std::vector<MVertex> vertices;
	vertices.clear();
	mds.clear();
	mt_files.clear();

	std::vector<CVector3D> t_vertices;
	std::vector<CVector2D> t_texcoords;
	std::vector<CVector3D> t_normals;

	// 按行读取 -- 获得 vertices 和 indices
	while ((*in)) {
		std::string t_line;
		std::getline((*in), t_line);
		std::replace(t_line.begin(), t_line.end(), ',', ' ');
		std::istringstream t_iss(t_line);

		std::string head; t_iss >> head;

		if (head.compare("v") == 0) {
			std::vector<float> list; float t_f;
			while (t_iss >> t_f) list.push_back(t_f);

			if (list.size() >= 3) t_vertices.push_back(CVector3D(list[0], list[1], list[2]));
		}
		else if (head.compare("vt") == 0) {
			std::vector<float> list; float t_f;
			while (t_iss >> t_f) list.push_back(t_f);
			if (list.size() >= 2) t_texcoords.push_back(CVector2D(list[0], list[1]));
		}
		else if (head.compare("vn") == 0) {
			std::vector<float> list; float t_f;
			while (t_iss >> t_f) list.push_back(t_f);
			if (list.size() >= 3) t_normals.push_back(CVector2D(list[0], list[1]));
		}
		else if (head.compare("f") == 0) {
			if (mds.size() == 0) {
				mds.push_back(MeshData());
			}
			std::vector<std::string> list; std::string t_s; 
			while (t_iss >> t_s) {
				std::replace(t_s.begin(), t_s.end(), '/', ' ');
				std::istringstream t_iss_f(t_s); 
				std::vector<uint> t_us; uint t_u;
				while (t_iss_f >> t_u) t_us.push_back(t_u-1);

				MVertex t_v;
				if (t_us.size() >= 1) t_v.position = t_vertices[t_us[0]];
				if (t_us.size() >= 2) t_v.tex_coord = t_texcoords[t_us[1]];
				if (t_us.size() >= 3) t_v.normal = t_normals[t_us[2]];
				vertices.push_back(t_v);

                mds.back().indices.push_back(static_cast<uint>(vertices.size() - 1));
			}

		}
		else if (head.compare("mtllib") == 0) {
			std::string t_path; t_iss >> t_path;
			mt_files.push_back(t_path);
		}
		else if (head.compare("usemtl") == 0) {
			std::string t_mt; t_iss >> t_mt;
			mds.push_back(MeshData());
			mds.back().material = t_mt;
		}

	}

	if (mds.size() == 0) { mds.push_back(MeshData()); }
	for(auto& md : mds) { md.vertices.assign(vertices.begin(), vertices.end()); }

	for (auto& md : mds) { gen_tangent(md); }

	if (source_type == SourceType::BY_FILE) { fs.close(); }

	return true;
}

bool MeshLoader::load_mesh_x(const std::string& path, std::vector<MeshData>& mds) {
	Assimp::Importer import;
	auto scene = import.ReadFile(path, aiProcess_ForceGenNormals | aiProcess_Triangulate /*| aiProcess_FlipUVs*/ | aiProcess_CalcTangentSpace /*| aiProcess_FlipWindingOrder*/);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		c_debuger() << "[error][mesh]load mesh fail\n" + std::string(import.GetErrorString());
		return false;
	}
	else { c_debuger() << "[yep][mesh]load mesh success\n" + path; }

	std::string s_path = FileHelper_ins().get_path_of_file(path);			
	mds.clear();

	// load mesh
	std::queue<aiNode*> nodes;
	nodes.push(scene->mRootNode);
	while (!nodes.empty()) {
		auto t_node = nodes.front(); nodes.pop();
		// load cur node's md
		for (uint i = 0; i < t_node->mNumMeshes; ++i) {
			auto t_mesh = scene->mMeshes[t_node->mMeshes[i]];
			// load one md;
			{
				MeshData md;

				// load vertice
				for (uint j = 0; j < t_mesh->mNumVertices; ++j) {
					MVertex t_v;

					auto t_mv = t_mesh->mVertices[j];
					t_v.position = CVector3D(t_mv.x, t_mv.y, t_mv.z);

					if (t_mesh->mTextureCoords[0] != nullptr) {
						auto t_mt = t_mesh->mTextureCoords[0][j];
						t_v.tex_coord = CVector2D(t_mt.x, t_mt.y);
					}

					if (t_mesh->mNormals != nullptr) {
						auto t_mn = t_mesh->mNormals[j];
						t_v.normal = CVector3D(t_mn.x, t_mn.y, t_mn.z);
					}

					if (t_mesh->mTangents != nullptr) {
						auto t_mt = t_mesh->mTangents[j];
						t_v.tangent = CVector3D(t_mt.x, t_mt.y, t_mt.z);
					} // tangent

					if (t_mesh->mBitangents != nullptr) {
						auto t_mt = t_mesh->mBitangents[j];
						t_v.bitangent = CVector3D(t_mt.x, t_mt.y, t_mt.z);
					} // bitangent

					md.vertices.push_back(t_v);
				}

				// load indices
				for (uint j = 0; j < t_mesh->mNumFaces; ++j) {
					auto t_face = t_mesh->mFaces[j];
					for (uint k = 0; k < t_face.mNumIndices; ++k) {
						md.indices.push_back(t_face.mIndices[k]);
					}
				}

				// load materials
				if (t_mesh->mMaterialIndex >= 0) {
					auto t_mt = scene->mMaterials[t_mesh->mMaterialIndex];

					aiString t_name;
					aiGetMaterialString(t_mt, AI_MATKEY_NAME, &t_name);
					md.material.name = t_name.C_Str();

					aiColor4D t_ka(1.0f, 1.0f, 1.0f, 1.0f);
					aiGetMaterialColor(t_mt, AI_MATKEY_COLOR_AMBIENT, &t_ka);
					md.material.ka = CVector3D(static_cast<float>(t_ka.r), static_cast<float>(t_ka.g), static_cast<float>(t_ka.b));

					aiColor4D t_kd(1.0f, 1.0f, 1.0f, 1.0f);
					aiGetMaterialColor(t_mt, AI_MATKEY_COLOR_DIFFUSE, &t_kd);
					md.material.kd = CVector3D(static_cast<float>(t_kd.r), static_cast<float>(t_kd.g), static_cast<float>(t_kd.b));

					aiColor4D t_ks(1.0f, 1.0f, 1.0f, 1.0f);
					aiGetMaterialColor(t_mt, AI_MATKEY_COLOR_SPECULAR, &t_ks);
					md.material.ks = CVector3D(static_cast<float>(t_ks.r), static_cast<float>(t_ks.g), static_cast<float>(t_ks.b));

					float t_sh;
					aiGetMaterialFloat(t_mt, AI_MATKEY_SHININESS, &t_sh);
					if (t_sh < 16.0f) t_sh = 16.0f;
					if (t_sh > 128.0f) t_sh = 128.0f;
					md.material.shininess = t_sh;

					// load texture
					{
						int len = t_mt->GetTextureCount(aiTextureType_AMBIENT);
						if (len > 1) len = 1;		// 临时
						for (int j = 0; j < len; ++j) {
							aiString t_txname;
							t_mt->GetTexture(aiTextureType_AMBIENT, j, &t_txname);
							md.material.map_ka = s_path + "/" + FileHelper_ins().get_name_of_file(t_txname.C_Str());
						}
					}
					{
						int len = t_mt->GetTextureCount(aiTextureType_DIFFUSE);
						if (len > 1) len = 1;		// 临时
						for (int j = 0; j < len; ++j) {
							aiString t_txname;
							t_mt->GetTexture(aiTextureType_DIFFUSE, j, &t_txname);
							md.material.map_kd = s_path + "/" + FileHelper_ins().get_name_of_file(t_txname.C_Str());
						}
					}
					{
						int len = t_mt->GetTextureCount(aiTextureType_SPECULAR);
						if (len > 1) len = 1;		// 临时
						for (int j = 0; j < len; ++j) {
							aiString t_txname;
							t_mt->GetTexture(aiTextureType_SPECULAR, j, &t_txname);
							md.material.map_ks = s_path + "/" + FileHelper_ins().get_name_of_file(t_txname.C_Str());
						}
					}
					{
						int len = t_mt->GetTextureCount(aiTextureType_NORMALS);
						if (len >= 1) {
							len = 1;				// 临时
							for (int j = 0; j < len; ++j) {
								aiString t_txname;
								t_mt->GetTexture(aiTextureType_NORMALS, j, &t_txname);
								md.material.map_normal = s_path + "/" + FileHelper_ins().get_name_of_file(t_txname.C_Str());
							}
						}
						else {
							len = t_mt->GetTextureCount(aiTextureType_HEIGHT);	// for obj
							if (len >= 1) {
								len = 1;				// 临时
								for (int j = 0; j < len; ++j) {
									aiString t_txname;
									t_mt->GetTexture(aiTextureType_HEIGHT, j, &t_txname);
									md.material.map_normal = s_path + "/" + FileHelper_ins().get_name_of_file(t_txname.C_Str());
								}
							}
						}
					} // normal map

					
				}

				mds.push_back(md);
			}
		}

		// push child node to queue
		for (uint i = 0; i < t_node->mNumChildren; ++i) { nodes.push(t_node->mChildren[i]); }

	}

	for (auto& md : mds) { gen_tangent(md); }
	return true;
}

bool MeshLoader::load_mesh_skeletal(const std::string& path, std::vector<SkeletalMeshData>& mds, MSkeleton& skeleton, std::vector<MBone>& bones, CMatrix4x4& mat_global, std::vector<AnimData>& ads) {
	Assimp::Importer import;
	auto scene = import.ReadFile(path, aiProcess_ForceGenNormals | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace /*| aiProcess_FlipWindingOrder*/);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		c_debuger() << "[error][skeletal_mesh]load skeletal_mesh fail\n" + path + " " + std::string(import.GetErrorString());
		return false;
	}
	else { c_debuger() << "[yep][skeletal_mesh]load skeletal_mesh success\n" + path; }

	std::string s_path = FileHelper_ins().get_path_of_file(path);
	mds.clear();
	skeleton.clear();
	skeleton.name = FileHelper_ins().get_name_of_file(path) + "_skeleton";
	bones.clear();
	ads.clear();

	// pre gen skeleton
	{
		std::queue<aiNode*> nodes;
		std::queue<int> ids;

		nodes.push(scene->mRootNode);
		ids.push(-1);		// 第一个是 root 节点

		while (!nodes.empty() && !ids.empty()) {
			auto t_node = nodes.front(); nodes.pop();
			int t_id = ids.front(); ids.pop();

			MSkeletonNode t_sn(t_node->mName.data, t_id);
			t_sn.mat_trans = aimat_to_cmat(t_node->mTransformation);
			skeleton.add_node(t_sn);

			for (uint i = 0; i < t_node->mNumChildren; ++i) {
				nodes.push(t_node->mChildren[i]);
				ids.push(static_cast<int>(skeleton.nodes.size()) - 1);
			}
		}
	}

	// load mesh
	mat_global = aimat_to_cmat(scene->mRootNode->mTransformation);
	mat_global = mat_global.inverse();

	std::queue<aiNode*> nodes;
	nodes.push(scene->mRootNode);
	while (!nodes.empty()) {
		auto t_node = nodes.front(); nodes.pop();
		// load cur node's md
		for (uint i = 0; i < t_node->mNumMeshes; ++i) {
			auto t_mesh = scene->mMeshes[t_node->mMeshes[i]];
			// load one md;
			{
				SkeletalMeshData md;

				// load vertice
				for (uint j = 0; j < t_mesh->mNumVertices; ++j) {
					MVertex t_v;

					auto t_mv = t_mesh->mVertices[j];
					t_v.position = CVector3D(t_mv.x, t_mv.y, t_mv.z);

					if (t_mesh->mTextureCoords[0] != nullptr) {
						auto t_mt = t_mesh->mTextureCoords[0][j];
						t_v.tex_coord = CVector2D(t_mt.x, t_mt.y);
					}

					if (t_mesh->mNormals != nullptr) {
						auto t_mn = t_mesh->mNormals[j];
						t_v.normal = CVector3D(t_mn.x, t_mn.y, t_mn.z);
					}
					
					if (t_mesh->mTangents != nullptr) {
						auto t_mt = t_mesh->mTangents[j];
						t_v.tangent = CVector3D(t_mt.x, t_mt.y, t_mt.z);
					} // tangent

					if (t_mesh->mBitangents != nullptr) {
						auto t_mt = t_mesh->mBitangents[j];
						t_v.bitangent = CVector3D(t_mt.x, t_mt.y, t_mt.z);
					} // bitangent

					md.vertices.push_back(t_v);
				}

				// load indices
				for (uint j = 0; j < t_mesh->mNumFaces; ++j) {
					auto t_face = t_mesh->mFaces[j];
					for (uint k = 0; k < t_face.mNumIndices; ++k) {
						md.indices.push_back(t_face.mIndices[k]);
					}
				}

				// load materials
				if (t_mesh->mMaterialIndex >= 0) {
					auto t_mt = scene->mMaterials[t_mesh->mMaterialIndex];

					aiString t_name;
					aiGetMaterialString(t_mt, AI_MATKEY_NAME, &t_name);
					md.material.name = t_name.C_Str();

					aiColor4D t_ka(1.0f, 1.0f, 1.0f, 1.0f);
					aiGetMaterialColor(t_mt, AI_MATKEY_COLOR_AMBIENT, &t_ka);
					md.material.ka = CVector3D(static_cast<float>(t_ka.r), static_cast<float>(t_ka.g), static_cast<float>(t_ka.b));

					aiColor4D t_kd(1.0f, 1.0f, 1.0f, 1.0f);
					aiGetMaterialColor(t_mt, AI_MATKEY_COLOR_DIFFUSE, &t_kd);
					md.material.kd = CVector3D(static_cast<float>(t_kd.r), static_cast<float>(t_kd.g), static_cast<float>(t_kd.b));

					aiColor4D t_ks(0.0f, 0.0f, 0.0f, 0.0f);
					aiGetMaterialColor(t_mt, AI_MATKEY_COLOR_SPECULAR, &t_ks);
					md.material.ks = CVector3D(static_cast<float>(t_ks.r), static_cast<float>(t_ks.g), static_cast<float>(t_ks.b));

					float t_sh;
					aiGetMaterialFloat(t_mt, AI_MATKEY_SHININESS, &t_sh);
					if (t_sh < 16.0f) t_sh = 16.0f;
					if (t_sh > 128.0f) t_sh = 128.0f;
					md.material.shininess = t_sh;

					// load texture
					{
						int len = t_mt->GetTextureCount(aiTextureType_AMBIENT);
						if (len > 1) len = 1;		// 临时
						for (int j = 0; j < len; ++j) {
							aiString t_txname;
							t_mt->GetTexture(aiTextureType_AMBIENT, j, &t_txname);
							md.material.map_ka = s_path + "/" + FileHelper_ins().get_name_of_file(t_txname.C_Str());
						}
					}
					{
						int len = t_mt->GetTextureCount(aiTextureType_DIFFUSE);
						if (len > 1) len = 1;		// 临时
						for (int j = 0; j < len; ++j) {
							aiString t_txname;
							t_mt->GetTexture(aiTextureType_DIFFUSE, j, &t_txname);
							md.material.map_kd = s_path + "/" + FileHelper_ins().get_name_of_file(t_txname.C_Str());
						}
					}
					{
						int len = t_mt->GetTextureCount(aiTextureType_SPECULAR);
						if (len > 1) len = 1;		// 临时
						for (int j = 0; j < len; ++j) {
							aiString t_txname;
							t_mt->GetTexture(aiTextureType_SPECULAR, j, &t_txname);
							md.material.map_ks = s_path + "/" + FileHelper_ins().get_name_of_file(t_txname.C_Str());
						}
					}
					{
						int len = t_mt->GetTextureCount(aiTextureType_NORMALS);
						if (len >= 1) {
							len = 1;				// 临时
							for (int j = 0; j < len; ++j) {
								aiString t_txname;
								t_mt->GetTexture(aiTextureType_NORMALS, j, &t_txname);
								md.material.map_normal = s_path + "/" + FileHelper_ins().get_name_of_file(t_txname.C_Str());
							}
						}
						else {
							len = t_mt->GetTextureCount(aiTextureType_HEIGHT);	// for obj
							if (len >= 1) {
								len = 1;				// 临时
								for (int j = 0; j < len; ++j) {
									aiString t_txname;
									t_mt->GetTexture(aiTextureType_HEIGHT, j, &t_txname);
									md.material.map_normal = s_path + "/" + FileHelper_ins().get_name_of_file(t_txname.C_Str());
								}
							}
						}
					} // normal map

				}

				// load bones
				md.bones.resize(md.vertices.size());					// 预分配顶点骨骼数据为顶点数据大小
				for (uint j = 0; j < t_mesh->mNumBones; ++j) {
					auto t_bone = t_mesh->mBones[j];

					// load skeleton's bones
					std::string t_bname = t_bone->mName.data;
					MBone t_b; t_b.mat_offset = aimat_to_cmat(t_bone->mOffsetMatrix);
					skeleton.add_bone(static_cast<int>(bones.size()), t_bname);
					bones.push_back(t_b);

					// load md's bones
					uint t_id = skeleton.get_node(t_bname).bone_id;		// 当前骨骼节点的下标
					if (t_id == -1) { continue; }
					for (uint k = 0; k < t_bone->mNumWeights; ++k) {
						uint v_id = t_bone->mWeights[k].mVertexId;
						float t_w = t_bone->mWeights[k].mWeight;
						md.bones[v_id].add(t_id, t_w);
					}
				}

				mds.push_back(md);
			}
		}

		// push child node to queue
		for (uint i = 0; i < t_node->mNumChildren; ++i) { nodes.push(t_node->mChildren[i]); }

	}

	// 生成切线
	for (auto& md : mds) { gen_tangent(md); }

	// load animation
	for (uint i = 0; i < scene->mNumAnimations; ++i) {
		AnimData t_ad;
		t_ad.name = scene->mAnimations[i]->mName.data;					// 可在 3dmax 中按 scene 名称保存
		t_ad.duration = static_cast<float>(scene->mAnimations[i]->mDuration);
		t_ad.ticks_per_seconds = static_cast<float>(scene->mAnimations[i]->mTicksPerSecond);

		// load anim node
		for (uint j = 0; j < scene->mAnimations[i]->mNumChannels; ++j) {
			auto t_c = scene->mAnimations[i]->mChannels[j];

			MAnimNode t_an;
			t_an.name = t_c->mNodeName.data;
			for (uint k = 0; k < t_c->mNumPositionKeys; ++k) {
				auto t_k = t_c->mPositionKeys[k];
				float t_t = static_cast<float>(t_k.mTime);
				CVector3D t_v = CVector3D(static_cast<float>(t_k.mValue.x), static_cast<float>(t_k.mValue.y), static_cast<float>(t_k.mValue.z));
				t_an.position_keys.push_back(MAnimPositionKey(t_t, t_v));
			}
			for (uint k = 0; k < t_c->mNumRotationKeys; ++k) {
				auto t_k = t_c->mRotationKeys[k];
				float t_t = static_cast<float>(t_k.mTime);
				CQuaternion t_v = CQuaternion(t_k.mValue.x, t_k.mValue.y, t_k.mValue.z, t_k.mValue.w);
				t_an.rotation_keys.push_back(MAnimRotationKey(t_t, t_v));
			}
			for (uint k = 0; k < t_c->mNumScalingKeys; ++k) {
				auto t_k = t_c->mScalingKeys[k];
				float t_t = static_cast<float>(t_k.mTime);
				CVector3D t_v = CVector3D(static_cast<float>(t_k.mValue.x), static_cast<float>(t_k.mValue.y), static_cast<float>(t_k.mValue.z));
				t_an.scale_keys.push_back(MAnimScaleKey(t_t, t_v));
			}

			t_ad.add_node(t_an);
		}

		ads.push_back(t_ad);
	}

	return true;
}

bool MeshLoader::load_mesh_animation(const std::string& path, std::vector<AnimData>& ads) {
	Assimp::Importer import;
	auto scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_FlipWindingOrder);

	if (!scene || !scene->mRootNode) {
		c_debuger() << "[error][animation]load animation fail\n" + path + " " + std::string(import.GetErrorString());
		return false;
	}
	else { c_debuger() << "[yep][animation]load animation success\n" + path; }

	std::string s_path = FileHelper_ins().get_path_of_file(path);
	ads.clear();

	for (uint i = 0; i < scene->mNumAnimations; ++i) {
		AnimData t_ad;
		t_ad.name = scene->mAnimations[i]->mName.data;					// 可在 3dmax 中按 scene 名称保存
		t_ad.duration = static_cast<float>(scene->mAnimations[i]->mDuration);
		t_ad.ticks_per_seconds = static_cast<float>(scene->mAnimations[i]->mTicksPerSecond);

		// load anim node
		for (uint j = 0; j < scene->mAnimations[i]->mNumChannels; ++j) {
			auto t_c = scene->mAnimations[i]->mChannels[j];

			MAnimNode t_an;
			t_an.name = t_c->mNodeName.data;
			for (uint k = 0; k < t_c->mNumPositionKeys; ++k) {
				auto t_k = t_c->mPositionKeys[k];
				float t_t = static_cast<float>(t_k.mTime);
				CVector3D t_v = CVector3D(static_cast<float>(t_k.mValue.x), static_cast<float>(t_k.mValue.y), static_cast<float>(t_k.mValue.z));
				t_an.position_keys.push_back(MAnimPositionKey(t_t, t_v));
			}
			for (uint k = 0; k < t_c->mNumRotationKeys; ++k) {
				auto t_k = t_c->mRotationKeys[k];
				float t_t = static_cast<float>(t_k.mTime);
				CQuaternion t_v = CQuaternion(t_k.mValue.x, t_k.mValue.y, t_k.mValue.z, t_k.mValue.w);
				t_an.rotation_keys.push_back(MAnimRotationKey(t_t, t_v));
			}
			for (uint k = 0; k < t_c->mNumScalingKeys; ++k) {
				auto t_k = t_c->mScalingKeys[k];
				float t_t = static_cast<float>(t_k.mTime);
				CVector3D t_v = CVector3D(static_cast<float>(t_k.mValue.x), static_cast<float>(t_k.mValue.y), static_cast<float>(t_k.mValue.z));
				t_an.scale_keys.push_back(MAnimScaleKey(t_t, t_v));
			}

			t_ad.add_node(t_an);
		}

		ads.push_back(t_ad);
	}

	return true;

}

void MeshLoader::gen_tangent(MeshData& md) {
	for (auto& v : md.vertices) { v.tangent = CVector3D(); }

	for (uint i = 0; i < md.indices.size(); i += 3) {
		auto& a = md.vertices[md.indices[i + 0]];
		auto& b = md.vertices[md.indices[i + 1]];
		auto& c = md.vertices[md.indices[i + 2]];

		CVector3D e1 = b.position - a.position;
		CVector3D e2 = c.position - a.position;

		CVector2D d1 = b.tex_coord - a.tex_coord;
		CVector2D d2 = c.tex_coord - a.tex_coord;

		float f = 1.0f / (d1.x() * d2.y() - d2.x() * d1.y());	// 行列式

		CVector3D t_tangent = f * ( d2.y() * e1 - d1.y() * e2);
		CVector3D t_bitange = f * (-d2.x() * e1 + d1.x() * e2);

		a.tangent += t_tangent; b.tangent += t_tangent; c.tangent += t_tangent;
		a.bitangent += t_bitange; b.bitangent += t_bitange; c.bitangent += t_bitange;
	}

	for (auto& v : md.vertices) { v.tangent.normalize(); v.bitangent.normalize(); }

}
void MeshLoader::gen_tangent(SkeletalMeshData& md) {
	for (auto& v : md.vertices) { v.tangent = CVector3D(); }

	for (uint i = 0; i < md.indices.size(); i += 3) {
		auto& a = md.vertices[md.indices[i + 0]];
		auto& b = md.vertices[md.indices[i + 1]];
		auto& c = md.vertices[md.indices[i + 2]];

		CVector3D e1 = b.position - a.position;
		CVector3D e2 = c.position - a.position;

		CVector2D d1 = b.tex_coord - a.tex_coord;
		CVector2D d2 = c.tex_coord - a.tex_coord;

		float f = 1.0f / (d1.x() * d2.y() - d2.x() * d1.y());	// 行列式

		CVector3D t_tangent = f * ( d2.y() * e1 - d1.y() * e2);
		CVector3D t_bitange = f * (-d2.x() * e1 + d1.x() * e2);

		a.tangent += t_tangent; b.tangent += t_tangent; c.tangent += t_tangent;
		a.bitangent += t_bitange; b.bitangent += t_bitange; c.bitangent += t_bitange;
	}

	for (auto& v : md.vertices) { v.tangent.normalize(); v.bitangent.normalize(); }

}

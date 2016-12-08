/*! @file main.cpp
 *  @brief apply compression on mesh to generate LoD
 */

#include <iostream>
#include "compression/Compression_Valence_Component.h"
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
    if (argc != 3) {
        cout << "Usage: " << argv[0]
             << " Input_file[input.p3d] Output_file_Prefix[output]" << endl;
        exit(1);
    }

    //!TODO
    //add parameter tree
    const char *input_file_name = argv[1];
    const char *output_file_name = argv[2];

    PolyhedronPtr m_ptr(new Polyhedron());

    //Decompression
    Compression_Valence_Component cv(m_ptr);
    cv.File_name = string(input_file_name);
    cv.Decompress_Init(*m_ptr);
    cout << "Total level of LoD is : " << cv.Total_layer << endl;

    size_t v_num(0), f_num(0);

    while (cv.Current_level != cv.Total_layer) {

        string s(output_file_name);
        char tmp[255];
        sprintf(tmp, "level%d.obj", cv.Current_level);
        s += string(tmp);
        ofstream ofs (s.c_str(), ofstream::out);
        size_t index = 0;
        for (Vertex_iterator i = m_ptr->vertices_begin(); i != m_ptr->vertices_end(); ++i) {
            ofs << "v "
                << i->point().x() << " "
                << i->point().y() << " "
                << i->point().z() << endl;
            i->tag(index++);
        }

        for (Facet_iterator i = m_ptr->facets_begin(); i != m_ptr->facets_end(); ++i) {
            ofs << "f";
            Halfedge_around_facet_circulator pHalfedge = i->facet_begin();
            do
                ofs << ' ' << pHalfedge->vertex()->tag() + 1;
            while (++pHalfedge != i->facet_begin());
            ofs << endl;
        }

        ofs.close();

        cv.Current_level = cv.Decompress_Each_Step(*m_ptr, cv.File_name.c_str());
    }

    return 0;
}

//int main(int argc, char** argv)
//{
//    if (argc != 4)
//    {
//        cout << "Usage: " << argv[0]
//             << " Input_file[obj] Output_file[p3d] Remain_V_Num" << endl;
//        exit(1);
//    }
//
//    char* input_file_name = argv[1];
//    char* output_file_name = argv[2];
//    int q_bit = 10;
//
//    int n_vertices;
//    sscanf(argv[3], "%d", &n_vertices);
//
//    bool is_normal_flipping_selected = false;
//    bool is_use_metric_selected = false;
//    float metric_thread = 0;
//    bool is_use_forget_metric_selected = false;
//    int forget_value = 0;
//    bool is_compression_selected = true;
//    bool is_adaptive_quantization_selected = false;
//    bool is_bijection_selected = false;
//
//    PolyhedronPtr mesh_ptr(new Polyhedron());
//    mesh_ptr->load_mesh_obj(input_file_name);
//
//    Compression_Valence_Component cv(mesh_ptr);
//    cv.Main_Function(
//            *mesh_ptr,
//            input_file_name,
//            output_file_name,
//            q_bit,
//            n_vertices,
//            is_normal_flipping_selected,
//            is_use_metric_selected,
//            metric_thread,
//            is_use_forget_metric_selected,
//            forget_value,
//            is_compression_selected,
//            is_adaptive_quantization_selected,
//            is_bijection_selected
//    );
//
//    return 0;
//}

#include "../../inc/linker/linker.hpp"
#include "../../inc/linker/env.hpp"

auto main(int argc,char** argv) -> int {

    env env_obj(argc, argv);

    Linker::init(
        env_obj.get_places(),
        env_obj.get_input_files(),
        env::hex, env::relocatable
    );

    Linker linker = Linker::get_instance();

    linker.load_files();
    // Uncomment if u want to see for each file its data
    // linker.print_files();

    // This loads section from all files
    // Sets them to 0
    // After that resolves relocations and symbols coralated with those sections
    linker.resolve_files();

    linker.print_linker_tables();

    // Now the output file
    // If hex was set with evv_bj linker has to solve all relocations and after that
    // Produce a hex file

    // If -relocatable was set, all linker has to do is to serilise his daa structures
    linker.produce_output(env_obj.get_file_name());

    return 0;
}


#include <transfer_function_plugin.hh>

std::map<std::string, TransferFunction_plugin_creator *> &
get_TransferFunction_plugin_map()
{
    static std::map<std::string, TransferFunction_plugin_creator *> TransferFunction_plugin_map;
    return TransferFunction_plugin_map;
}

void print_TransferFunction_plugins()
{
    std::map<std::string, TransferFunction_plugin_creator *> &m = get_TransferFunction_plugin_map();
    std::map<std::string, TransferFunction_plugin_creator *>::iterator it;
    it = m.begin();
    csoca::ilog << "- Available transfer function plug-ins:" << std::endl;
    while (it != m.end())
    {
        if ((*it).second)
            csoca::ilog << "\t\'" << (*it).first << "\'" << std::endl;
        ++it;
    }
}

TransferFunction_plugin *select_TransferFunction_plugin(ConfigFile &cf)
{
    std::string tfname = cf.GetValue<std::string>("cosmology", "transfer");

    TransferFunction_plugin_creator *the_TransferFunction_plugin_creator = get_TransferFunction_plugin_map()[tfname];

    if (!the_TransferFunction_plugin_creator)
    {
        csoca::elog << "Invalid/Unregistered transfer function plug-in encountered : " << tfname << std::endl;
        print_TransferFunction_plugins();
        throw std::runtime_error("Unknown transfer function plug-in");
    }
    else
    {
        csoca::ilog << "Selecting transfer function plug-in \'" << tfname << "\'..." << std::endl;
    }

    TransferFunction_plugin *the_TransferFunction_plugin = the_TransferFunction_plugin_creator->create(cf);

    return the_TransferFunction_plugin;
}

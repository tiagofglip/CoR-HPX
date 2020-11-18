//#include "cor/cor.hpp"



#include <hpx/hpx.hpp>

#include "cor/services/controller.hpp"

#include "cor/utils/utils.hpp"
#include "cor/services/page_manager.hpp"
#include "cor/services/session_manager.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include "cor/elements/mailbox_client.hpp"

// #include <thread>
// #include <sstream>

// #include <iostream> // to remove


namespace cor {

Controller::Controller(std::string const& id, std::string const& app_group, std::string const& context, unsigned int npods) :
    _app_group{app_group},
    _context{context},
    _npods{npods},
    _init_total_npods{0},
    _final_total_npods{0},
    _init_ctx_npods{0},
    _final_ctx_npods{0},
    _cv{},
    _mtx{},
    _pg_mgr{nullptr},
    _rsc_mgr{nullptr},
    _sess_mgr{nullptr}
{
    idpManager_object = new IdpManager_Client();
    resourceManagerGlobal_object = new ResourceManagerGlobal_Client();
    accessManager_object = new AccessManager_Client(context);

    // Atualizar o ip e a porta do servidor agas
    agas_address = hpx::get_config_entry("hpx.agas.address", HPX_INITIAL_IP_ADDRESS);
    agas_port = std::stoi(hpx::get_config_entry("hpx.agas.port", std::to_string(HPX_INITIAL_IP_PORT)));

    // Atualizar o ip e a porta do hpx desta localidade
    hpx_address = hpx::get_config_entry("hpx.parcel.address", HPX_INITIAL_IP_ADDRESS);
    hpx_port = std::stoi(hpx::get_config_entry("hpx.parcel.port", std::to_string(HPX_INITIAL_IP_PORT)));
    // std::cout << "AddConnection " << hpx_address << ":" << hpx_port << std::endl; 
}

void Controller::StartService()
{
    // initialize global context
    Initialize();

    // // create page manager initial context
    _pg_mgr->CreateInitialContext();

    // // create resource manager initial context
    _rsc_mgr->CreateInitialContext(GetName());

    // adicionar esta localidade ao map de localidades deste contexto
    accessManager_object->AddMyContextLocality(_context, hpx::get_locality_id());

    // Adicionar esta conecção
    accessManager_object->AddConnection(hpx_address, hpx_port);
    
}


void Controller::StopService()
{
    Finalize();
}


void Controller::Initialize()
{
    std::cout << "Controller::Initialize()" << std::endl;
    // join app group and synchronize
    // _mbox->join(_app_group);
    bool _is_main_ctrl=false;
    if(hpx::get_locality_id()==0)
        _is_main_ctrl = true;

    // // create page and resource managers
    _pg_mgr = new PageManager(this);
    _rsc_mgr = new ResourceManager(this, _is_main_ctrl);
    _sess_mgr = new SessionManager();
}

void Controller::Finalize()
{
    _sess_mgr->StopService();
    //accessManager_object->RemoveConnection(host, port);
}

idp_t Controller::GenerateIdp()
{
    return _pg_mgr->GenerateIdp();
}

void Controller::InsertIdp(idp_t idp, hpx::id_type gid)
{
    return _rsc_mgr->InsertIdp(idp, gid);
}

bool Controller::FindIdp(idp_t idp)
{
    return _rsc_mgr->FindIdp(idp);
}

hpx::id_type Controller::GetGidFromIdp(idp_t idp)
{
    return _rsc_mgr->GetGidFromIdp(idp);
}

void Controller::RemoveIdp(idp_t idp)
{
    return _rsc_mgr->RemoveIdp(idp);
}

void Controller::InsertIdpGlobal(idp_t idp, hpx::id_type gid)
{
    return resourceManagerGlobal_object->InsertIdp(idp, gid);
}

bool Controller::FindIdpGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->FindIdpGlobal(idp);
}

hpx::id_type Controller::GetGidFromIdpGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->GetGidFromIdp(idp);
}

void Controller::RemoveIdpGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->RemoveIdp(idp);
}

std::string Controller::GetGlobalContext()
{
    return _app_group;
}

std::string Controller::GetLocalContext()
{
    return _context;
}

unsigned int Controller::GetTotalPods()
{    
    _mtx2.lock();
    auto res = _init_total_npods;
    _mtx2.unlock();

    return res;
}

unsigned int Controller::GetTotalDomains()
{
    return _rsc_mgr->GetTotalDomains();
}

idp_t Controller::GetDomainIdp(idp_t idp)
{
    return _rsc_mgr->GetDomainIdp(idp);
}

void Controller::InsertPredecessorIdp(idp_t idp, idp_t idp_predecessor)
{
    return _rsc_mgr->InsertPredecessorIdp(idp, idp_predecessor);
}

idp_t Controller::GetPredecessorIdp(idp_t idp)
{
    return _rsc_mgr->GetPredecessorIdp(idp);
}

bool Controller::FindPredecessorIdp(idp_t idp)
{
    return _rsc_mgr->FindPredecessorIdp(idp);
}

void Controller::RemovePredecessorIdp(idp_t idp)
{
    return _rsc_mgr->RemovePredecessorIdp(idp);
}

void Controller::InsertPredecessorIdpGlobal(idp_t idp, idp_t idp_predecessor)
{
    return resourceManagerGlobal_object->InsertPredecessorIdp(idp, idp_predecessor);
}

idp_t Controller::GetPredecessorIdpGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->GetPredecessorIdp(idp);
}

bool Controller::FindPredecessorIdpGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->FindPredecessorIdp(idp);
}

void Controller::RemovePredecessorIdpGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->RemovePredecessorIdp(idp);
}



void Controller::InsertDynamicOrganizer_idps(idp_t idp)
{
    std::cout << "cacacac4" << std::endl;
    return _rsc_mgr->InsertDynamicOrganizer_idps(idp);
}

void Controller::InsertStaticOrganizer_idps(idp_t idp)
{
    return _rsc_mgr->InsertStaticOrganizer_idps(idp);
}

bool Controller::FindDynamicOrganizer_idps(idp_t idp)
{
    return _rsc_mgr->FindDynamicOrganizer_idps(idp);
}

bool Controller::FindStaticOrganizer_idps(idp_t idp)
{
    return _rsc_mgr->FindStaticOrganizer_idps(idp);
}

void Controller::InsertAgentMailbox(idp_t idp, hpx::id_type gid)
{
    return _rsc_mgr->InsertAgentMailbox(idp, gid);
}

hpx::id_type Controller::GetAgentMailbox(idp_t idp)
{
    return _rsc_mgr->GetAgentMailbox(idp);
}

void Controller::InsertDynamicOrganizer_idpsGlobal(idp_t idp)
{
    std::cout << "cacacac5" << std::endl;
    return resourceManagerGlobal_object->InsertDynamicOrganizer_idps(idp);
}

void Controller::InsertStaticOrganizer_idpsGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->InsertStaticOrganizer_idps(idp);
}

bool Controller::FindDynamicOrganizer_idpsGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->FindDynamicOrganizer_idps(idp);
}

bool Controller::FindStaticOrganizer_idpsGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->FindStaticOrganizer_idps(idp);
}

void Controller::InsertAgentMailboxGlobal(idp_t idp, hpx::id_type gid)
{
    return resourceManagerGlobal_object->InsertAgentMailbox(idp, gid);
}

hpx::id_type Controller::GetAgentMailboxGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->GetAgentMailbox(idp);
}




idp_t Controller::Spawn(std::string const& context, unsigned int npods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
{

    // Para os processos filho saberem quem é o primeiro, é adicionado um novo context ao accessManager_object
    accessManager_object->AddContext(context);

    // spawn pods
    for (int i = 0; i < npods; ++i) {
    auto pos = i % hosts.size();

    // assemble command
    std::string cmd;
    cmd.append("/opt/placor-hpx/bin/corhpx");
    cmd.append(" ");
    cmd.append(_app_group);
    cmd.append(" ");
    cmd.append(context);
    cmd.append(" ");
    cmd.append(std::to_string(npods));
    cmd.append(" ");
    cmd.append(std::to_string(parent));
    cmd.append(" ");
    cmd.append(module);
    for (int i = 0; i < args.size(); ++i) {
        cmd.append(" ");
        cmd.append(args[i]);
    }
    cmd.append(" ");

    // hpx command lines
    unsigned int port_int = accessManager_object->GetNextPort(hosts[pos]); // buscar nova porta para o filho
    std::string host = hosts[pos];
    std::string port = std::to_string(port_int);
        
    cmd.append("--hpx:hpx=" + host + ":" + port);
    // cmd.append("--hpx:hpx=" + host + ":1338");
    cmd.append(" ");
    cmd.append("--hpx:agas=" + agas_address + ":" + std::to_string(agas_port));
    // cmd.append("--hpx:agas=localhost:" + std::to_string(agas_port));
    cmd.append(" ");
    cmd.append("--hpx:run-hpx-main");
    cmd.append(" ");
    cmd.append("--hpx:expect-connecting-localities");
    cmd.append(" ");
    cmd.append("--hpx:worker");
    cmd.append(" ");
    cmd.append("--hpx:threads=2");
    cmd.append(" ");
    cmd.append("--hpx:ini=hpx.component_paths=/opt/placor-hpx/examples");

    std::cout << "spawned comando: " << cmd << std::endl;


    _sess_mgr->CreateRemoteSession(host, "22", cmd);

    }

    hpx::id_type my_mailBox_gid = GetAgentMailbox(parent);
    Mailbox_Client my_mailbox(std::move(my_mailBox_gid));
    cor::Message msg;
    idp_t clos_idp = my_mailbox.ReceiveNewClos(context);
    return clos_idp;





    // // Para os processos filho saberem quem é o primeiro, é adicionado um novo context ao accessManager_object
    // accessManager_object->AddContext(context);

  

    // for(int i=0; i < npods; i++) {
    //     auto pos = i % hosts.size();
    //     unsigned int port_int = accessManager_object->GetNextPort(hosts[pos]); // buscar nova porta para o filho

    //     int pid = fork();
    //     if (pid == 0) {
    //         std::cout << "I'm the child number " << i+1 << std::endl; 
    //         // // // assemble command
    //         std::vector<char*> exec_args;

    //         char binaryPath1[] = "/opt/placor-hpx/bin/corhpx";
    //         char *binaryPath = binaryPath1;
    //         exec_args.push_back(binaryPath);
            
    //         char * arg1 = new char [_app_group.length()+1];
    //         std::strcpy (arg1, _app_group.c_str());
    //         exec_args.push_back(arg1);

    //         char * arg2 = new char [context.length()+1];
    //         std::strcpy (arg2, context.c_str());
    //         exec_args.push_back(arg2);

    //         char * arg3 = new char [std::to_string(npods).length()+1];
    //         std::strcpy (arg3, std::to_string(npods).c_str());
    //         exec_args.push_back(arg3);

    //         char * arg4 = new char [std::to_string(parent).length()+1];
    //         std::strcpy (arg4, std::to_string(parent).c_str());
    //         exec_args.push_back(arg4);

    //         char * arg5 = new char [module.length()+1];
    //         std::strcpy (arg5, module.c_str());
    //         exec_args.push_back(arg5);

    //         for (int i = 0; i < args.size(); ++i) {
    //             char * arg = new char [args[i].length()+1];
    //             std::strcpy (arg, args[i].c_str());
    //             exec_args.push_back(arg);
    //         }
 
    //         // hpx command lines
        
    //         std::string host = hosts[pos];
            
    //         std::string port = std::to_string(port_int);
             
    //         std::string arg66 = "--hpx:hpx=" + host + ":" + port;
    //         //std::string arg66 = "--hpx:hpx=" + hosts[pos];
    //         std::string arg77 = "--hpx:agas=" + agas_address + ":" + std::to_string(agas_port);
    //         std::string arg88 = "--hpx:run-hpx-main";
    //         std::string arg99 = "--hpx:expect-connecting-localities";
    //         std::string arg100 = "--hpx:worker";
    //         std::string arg110 = "--hpx:threads=2";
    //         // std::cout << "spawn_agas-> " << arg77 << std::endl;
    //         // std::cout << "spawn_host-> " << arg66 << std::endl;
    //         char * arg6 = new char [arg66.length()+1];
    //         std::strcpy (arg6, arg66.c_str());
    //         exec_args.push_back(arg6);

    //         char * arg7 = new char [arg77.length()+1];
    //         std::strcpy (arg7, arg77.c_str());
    //         exec_args.push_back(arg7);

    //         char * arg8 = new char [arg88.length()+1];
    //         std::strcpy (arg8, arg88.c_str());
    //         exec_args.push_back(arg8);

    //         char * arg9 = new char [arg99.length()+1];
    //         std::strcpy (arg9, arg99.c_str());
    //         exec_args.push_back(arg9);

    //         char * arg10 = new char [arg100.length()+1];
    //         std::strcpy (arg10, arg100.c_str());
    //         exec_args.push_back(arg10);

    //         char * arg11 = new char [arg110.length()+1];
    //         std::strcpy (arg11, arg110.c_str());
    //         exec_args.push_back(arg11);

    //         exec_args.push_back(NULL);
    //         // printf("AAQUII - 2\n");

    //         for(int j = 0; j < exec_args.size(); j++)
    //         std::cout << exec_args[j] << std::endl;
    //         // std::cout << binaryPath << " " << arg1 << " " << arg2 << " " << arg3 << " " << arg4 << " " << arg5 << " " << arg6 << " " << arg7 << " " << arg8 << " " << arg9 << " " << arg10 << std::endl;
    //         // //auto res = system(cmd.c_str());
    //         // //execl(cmd.c_str(), cmd.c_str(), nullptr);
    //         // //execl(binaryPath, binaryPath, arg1, arg2, arg3, arg4, arg5, arg6, arg8, arg9, nullptr);
    //         // execl(binaryPath, binaryPath, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, nullptr);
    //         std::cout << "execl" << std::endl;
    //         //execl("/opt/placor-hpx/bin/corhpx", "/opt/placor-hpx/bin/corhpx", "app", "ctx2", "2", "4294967038", "/opt/placor-hpx/examples/libspawn.so", "--hpx:hpx=localhost:1339", "--hpx:agas=localhost:1337", "--hpx:run-hpx-main", "--hpx:expect-connecting-localities", "--hpx:worker", "--hpx:threads=2", "--hpx:ini=hpx.component_paths=/opt/placor-hpx/examples", nullptr);      
    //         execv(binaryPath, exec_args.data());
    //         printf ("Erro no execl\n");
    //         exit(-1);
    //     }
    //     else if (pid < 0)
    //     {
    //         // Error forking, still in parent process (there are no child process at this point)
    //         std::cout << "Fork error: " << errno << ", " << strerror(errno) << '\n';
    //     }

    // } // end for



    return 1;


}





std::string Controller::GetName() const
{
    return "naoseiquetediga";
}

std::string Controller::SearchResource(idp_t idp)
{
    return _rsc_mgr->SearchResource(idp);
}

bool Controller::ContainsResource(idp_t idp)
{
    return _rsc_mgr->ContainsResource(idp);
}

// Recebe o pedido do page_manager e envia a nova página proveniente do componente "idpManager_basename"
page_t Controller::RequestPage()
{
    page_t page = idpManager_object->GeneratePage();

    return page;
}








}

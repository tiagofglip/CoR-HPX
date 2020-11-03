
#include <hpx/hpx.hpp>
#include "cor/system/pod.hpp"
#include "cor/utils/utils.hpp"

#include "cor/resources/domain.hpp"

using namespace dll;

namespace cor {

Pod::Pod(std::string const& id, std::string const& app_group, std::string const& context, unsigned int npods) :
    _ctrl{nullptr},
    _modules{},
    _active_rscs{}
{
    _ctrl = new Controller(id, app_group, context, npods);
}

Pod::~Pod()
{
    // delete _ctrl;
    // delete _mlr;
    // delete _mlr_hpx;
}

void Pod::Initialize()
{
    // start controller service
    _ctrl->StartService();
}

void Pod::Finalize()
{
    // stop controller services
    _ctrl->StopService();
}

std::string Pod::GetGlobalContext()
{
    return _ctrl->GetGlobalContext();
}

std::string Pod::GetLocalContext()
{
    return _ctrl->GetLocalContext();
}

unsigned int Pod::GetTotalPods()
{    
    return _ctrl->GetTotalPods();
}

unsigned int Pod::GetTotalDomains()
{
    return _ctrl->GetTotalDomains();
}

idp_t Pod::GetActiveResourceIdp(size_t id) {
    // std::cout << "Thread HPX - GetActiveResourceIdp2: " << id << std::endl;
    idp_t nnn = _active_rscs.at(id);

    return nnn;
}

idp_t Pod::GetDomainIdp1()
{
    return domain_idp;
}

idp_t Pod::GetDomainIdp2(idp_t idp)
{
    return _ctrl->GetDomainIdp(idp);
}

void Pod::setDomainIdp(idp_t idp)
{
    domain_idp = idp;
}

idp_t Pod::GetPredecessorIdp(idp_t idp)
{
    return _ctrl->GetPredecessorIdp(idp);
}

idp_t Pod::Spawn(std::string const& context, unsigned int npods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
{
    return _ctrl->Spawn(context, npods, parent, module, args, hosts);
}

void Pod::LoadModule(std::string const& module)
{
    // std::unique_lock<std::mutex> lk(_mtx);
    if (_modules.find(module) == _modules.end()) {
        auto dylib = DynamicLoader::LoadDynamicLibrary(module);
        _modules.emplace(utils::get_filename(module), dylib);
    }
}

void Pod::InsertActiveResource(size_t tid, idp_t idp)
{
    std::unique_lock<std::mutex> lk(_mtx);
    if (_active_rscs.find(tid) == _active_rscs.end())
        _active_rscs.emplace(tid, idp);
}

void Pod::RemoveActiveResource(size_t tid)
{
    std::unique_lock<std::mutex> lk(_mtx);
    _active_rscs.erase(tid);
}

void Pod::ChangeActiveResource(size_t tid, idp_t idp)
{
    std::unique_lock<std::mutex> lk(_mtx);
    _active_rscs.at(tid) = idp;
}

idp_t Pod::GetCurrentActiveResource(size_t tid)
{
    std::unique_lock<std::mutex> lk(_mtx);
    return _active_rscs.at(tid);
}

std::string Pod::SearchResource(idp_t idp)
{
    return _ctrl->SearchResource(idp);
}

bool Pod::ContainsResource(idp_t idp)
{
    return _ctrl->ContainsResource(idp);
}

}


typedef cor::Pod Pod;
typedef hpx::components::component<cor::Pod> Pod_type;

HPX_REGISTER_COMPONENT(Pod_type, Pod);


typedef cor::Pod::Initialize_action_pod Initialize_action_pod;
typedef cor::Pod::Finalize_action_pod Finalize_action_pod;
typedef cor::Pod::GetGlobalContext_action_pod GetGlobalContext_action_pod;
typedef cor::Pod::GetLocalContext_action_pod GetLocalContext_action_pod;
typedef cor::Pod::GetTotalPods_action_pod GetTotalPods_action_pod;
typedef cor::Pod::GetTotalDomains_action_pod GetTotalDomains_action_pod;
typedef cor::Pod::GetActiveResourceIdp_action_pod GetActiveResourceIdp_action_pod;
typedef cor::Pod::GetPredecessorIdp_action_pod GetPredecessorIdp_action_pod;
typedef cor::Pod::SearchResource_action_pod SearchResource_action_pod;
typedef cor::Pod::ContainsResource_action_pod ContainsResource_action_pod;
typedef cor::Pod::setDomainIdp_action_pod setDomainIdp_action_pod;
typedef cor::Pod::LoadModule_action_pod LoadModule_action_pod;
typedef cor::Pod::GetDomainIdp1_action_pod GetDomainIdp1_action_pod;
typedef cor::Pod::GetDomainIdp2_action_pod GetDomainIdp2_action_pod;
typedef cor::Pod::InsertActiveResource_action_pod InsertActiveResource_action_pod;
typedef cor::Pod::RemoveActiveResource_action_pod RemoveActiveResource_action_pod;
typedef cor::Pod::ChangeActiveResource_action_pod ChangeActiveResource_action_pod;
typedef cor::Pod::GetCurrentActiveResource_action_pod GetCurrentActiveResource_action_pod;
typedef cor::Pod::Spawn_action_pod Spawn_action_pod;


HPX_REGISTER_ACTION(Initialize_action_pod);
HPX_REGISTER_ACTION(Finalize_action_pod);
HPX_REGISTER_ACTION(GetGlobalContext_action_pod);
HPX_REGISTER_ACTION(GetLocalContext_action_pod);
HPX_REGISTER_ACTION(GetTotalPods_action_pod);
HPX_REGISTER_ACTION(GetTotalDomains_action_pod);
HPX_REGISTER_ACTION(GetActiveResourceIdp_action_pod);
HPX_REGISTER_ACTION(GetPredecessorIdp_action_pod);
HPX_REGISTER_ACTION(SearchResource_action_pod);
HPX_REGISTER_ACTION(ContainsResource_action_pod);
HPX_REGISTER_ACTION(setDomainIdp_action_pod);
HPX_REGISTER_ACTION(LoadModule_action_pod);
HPX_REGISTER_ACTION(GetDomainIdp1_action_pod);
HPX_REGISTER_ACTION(GetDomainIdp2_action_pod);
HPX_REGISTER_ACTION(InsertActiveResource_action_pod);
HPX_REGISTER_ACTION(RemoveActiveResource_action_pod);
HPX_REGISTER_ACTION(ChangeActiveResource_action_pod);
HPX_REGISTER_ACTION(GetCurrentActiveResource_action_pod);
HPX_REGISTER_ACTION(Spawn_action_pod);
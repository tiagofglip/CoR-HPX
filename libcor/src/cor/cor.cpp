


#include "cor/cor.hpp"

#include "cor/utils/utils.hpp"
// #include "cor/system/pod.hpp"
// #include "cor/system/rpc_manager.hpp"
// #include "cor/external/event/event.hpp"

// #include "cor/resources/domain_component.hpp"
#include "cor/system/pod_client.hpp"
// #include "cor/resource/domain_component_client.hpp"

namespace cor {

// hpx::id_type Initialize_hpx(std::string const& app_group, std::string const& context, unsigned int npods, std::string const& module)
std::unique_ptr<Domain_Client> Initialize_hpx(std::string const& app_group, std::string const& context, unsigned int npods, std::string const& module)
{

    // generate a random id for cor services
    auto id = utils::random_string(9);


    // auto index = global::pod_index.fetch_add(1);
    // global::pods[index] = new Pod_Client(id, index, app_group, context, npods); // not thread safe
    // global::pods[index]->Initialize();

    global::_mtx.lock();
    //std::cout << "entrei" << std::endl;
    //std::this_thread::sleep_for (std::chrono::seconds(1));
    unsigned int index = global::pods.size();
    std::cout << "criado um pod com id: " << index << std::endl;
    global::pods.push_back(new Pod_Client(id, index, app_group, context, npods)); // not thread safe
    //std::cout << "saí" << std::endl;
    global::_mtx.unlock();
    global::pods[index]->Initialize();


    return global::pods[index]->CreateLocal<Domain_Client>(cor::MetaDomain, "", module);


}

void Finalize_hpx()
{
    // global::pod->Finalize();
}

// hpx::future<std::shared_ptr<Domain_Client>> GetDomain(hpx::launch::async_policy)
// {
//     return hpx::async([](){
//         idp_t domain_idp = global::pod->GetDomainIdp();
//         // std::cout << "domain_idp: " << domain_idp << std::endl;
//         auto dom = global::pod->GetLocalResource<Domain_Client>(domain_idp);
//         std::shared_ptr<Domain_Client> dom_shared = std::move(dom);
//         dom_shared->GetActiveResourceIdp(hpx::launch::async).get();
//         return dom_shared;
//     });
// }

// std::shared_ptr<Domain_Client> GetDomain()
// {
//     idp_t domain_idp = global::pod->GetDomainIdp();
//     // std::cout << "domain_idp: " << domain_idp << std::endl;
//     auto dom = global::pod->GetLocalResource<Domain_Client>(domain_idp);
//     std::shared_ptr<Domain_Client> dom_shared = std::move(dom);
//     dom_shared->GetActiveResourceIdp();

//     return dom_shared;
// }


std::shared_ptr<Domain_Client> GetDomain()
{
    std::cout << "GetDomain 1" << std::endl;

    auto id = hpx::threads::get_self_id();
    uint64_t* ptr=(uint64_t*) &id;
    auto id_hpx_thread = (*ptr);

    std::cout << "GetDomain 2" << std::endl;

    auto pod_id = global::active_rscs[id_hpx_thread];

    std::cout << "dominio " << pod_id << " com id_task " << id_hpx_thread << std::endl;

    idp_t domain_idp = global::pods[pod_id]->GetDomainIdp();
    // std::cout << "domain_idp: " << domain_idp << std::endl;
    auto dom = global::pods[pod_id]->GetLocalResource<Domain_Client>(domain_idp);
    std::shared_ptr<Domain_Client> dom_shared = std::move(dom);
    dom_shared->GetActiveResourceIdp();
    std::cout << "GetDomain 3" << std::endl;
    return dom_shared;


}




}

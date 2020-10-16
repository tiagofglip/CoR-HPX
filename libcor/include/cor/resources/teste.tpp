#ifdef COR_TESTE_HPP

// #include "cor/resources/teste.hpp"

namespace cor {


template <typename T>
std::unique_ptr<T> Teste::GetLocalResource(idp_t idp)
{

    // if(hpx::get_locality_id()==0) {
    //     std::vector<hpx::id_type> localities = hpx::find_remote_localities();
        
    //     std::cout << "Minha localidade: " << hpx::find_here() << std::endl;

    //     auto id_type = hpx::new_<Teste>(hpx::find_here()).get();

    //     for (hpx::id_type const& id : localities)
    //     {
    //         std::cout << "Localidade remota: " << id << std::endl;
    //         // std::cout << "O Dado está na localidade: " << data->GetLocalityGID() << std::endl;
    //         auto newgid = hpx::components::migrate<Teste>(id_type, id).get();

    //         std::cout << "O Dado está na localidade: " << hpx::get_colocation_id(hpx::launch::sync, newgid) << std::endl;

    //     }
    // }


    return container.GetLocalResource<T>(idp);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Teste::CreateLocal(idp_t ctx, std::string const& name, Args&& ... args)
{

    // if(hpx::get_locality_id()==0) {
    //     std::vector<hpx::id_type> localities = hpx::find_remote_localities();
        
    //     std::cout << "Minha localidade: " << hpx::find_here() << std::endl;

    //     auto id_type = hpx::new_<Teste>(hpx::find_here()).get();

    //     for (hpx::id_type const& id : localities)
    //     {
    //         std::cout << "Localidade remota: " << id << std::endl;
    //         // std::cout << "O Dado está na localidade: " << data->GetLocalityGID() << std::endl;
    //         auto newgid = hpx::components::migrate<Teste>(id_type, id).get();

    //         std::cout << "O Dado está na localidade: " << hpx::get_colocation_id(hpx::launch::sync, newgid) << std::endl;

    //     }
    // }


    return container.CreateLocal<T, Args...>(ctx, name, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
idp_t Teste::CreateRemote(idp_t ctx, std::string const& name, Args&& ... args)
{
    return container.CreateRemote<T, Args...>(ctx, name, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
idp_t Teste::Create(idp_t ctx, std::string const& name, Args&& ... args)
{
    return container.Create<T>(ctx, name, std::forward<Args>(args)...);
}

template <typename T>
std::unique_ptr<T> Teste::CreateReference(idp_t idp, idp_t ctx, std::string const& name)
{
    return container.CreateReference<T>(idp, ctx, name);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Teste::CreateCollective1(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args)
{
    return container.CreateCollective<T, Args...>(ctx, name, total_members, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Teste::CreateCollective2(idp_t active_rsc_idp, idp_t clos, idp_t ctx, std::string const& name, Args&& ... args)
{

    // if(hpx::get_locality_id()==0) {
    //     std::vector<hpx::id_type> localities = hpx::find_remote_localities();
        
    //     std::cout << "Minha localidade: " << hpx::find_here() << std::endl;

    //     auto id_type = hpx::new_<Teste>(hpx::find_here()).get();

    //     for (hpx::id_type const& id : localities)
    //     {
    //         std::cout << "Localidade remota: " << id << std::endl;
    //         // std::cout << "O Dado está na localidade: " << data->GetLocalityGID() << std::endl;
    //         auto newgid = hpx::components::migrate<Teste>(id_type, id).get();

    //         std::cout << "O Dado está na localidade: " << hpx::get_colocation_id(hpx::launch::sync, newgid) << std::endl;

    //     }
    // }

    return container.CreateCollective<T, Args...>(active_rsc_idp, clos, ctx, name, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
void Teste::Run(idp_t idp, Args&&... args)
{
    return container.Run<T, Args...>(idp, std::forward<Args>(args)...);
}

template <typename T>
void Teste::Wait(idp_t idp)
{
    return container.Wait<T>(idp);
}

template <typename T>
auto Teste::Get(idp_t idp)
{
    return container.Get<T>(idp);
}

}

#endif
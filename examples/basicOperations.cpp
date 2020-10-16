#include "cor/cor.hpp"

extern "C"
{
    void Main(int rsc_idp);
}

void Main(int rsc_idp)
{
    // obter o domínio local
    auto domain = cor::GetDomain();

    // obter o idp do agent que está a executar a função de entrada do módulo do utilizador
    auto agent_idp = domain->GetActiveResourceIdp();

    // obter uma referência para o agente
    auto agent = domain->GetLocalResource<cor::ProtoAgent_Client<void(int,char**)>>(agent_idp);

    // criar um grupo para introduzir um novo módulo do utilizador na aplicação
    std::string const& path = "/opt/placor-hpx/examples/libcallableModule.so"; // é necessario fazer estas atribuições porque os argumentos de componentes tem de ser const
    auto group = domain->CreateLocal<cor::Group_Client>(domain->Idp(), "group", path);

    // criar um dado no qual irá ser escrito o idp do agente que irá ser criado
    auto data = domain->CreateLocal<cor::Data_Client<idp_t>>(group->Idp(), "data");

    // criar um agente que irá carregar dinamicamente e executar uma função presente no novo módulo do utilizador
    std::string const& func = "Test";
    auto new_agent = domain->CreateLocal<cor::ProtoAgent_Client<idp_t(idp_t)>>(group->Idp(), "agent", group->GetModuleName(), func);
    // executa a função solicitada, passando os parâmetros correspondentes
    idp_t const& agent_idpp = agent_idp ;
    new_agent->Run(agent_idpp);
    // espera pelo término da execução da função
    new_agent->Wait();
    // obtém o resultado da execução da função
    auto res1 = new_agent->Get();

    data->Acquire();
    auto res2 = **data;
    data->Release();

    std::cout << res1 << "\t" << res2 << "\n";
}
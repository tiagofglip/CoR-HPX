#include "cor/elements/dynamic_organizer.hpp"

#include "cor/system/system.hpp"
#include "cor/system/pod_client.hpp"
#include "cor/utils/utils.hpp"

#include <algorithm>


namespace cor {

DynamicOrganizer::DynamicOrganizer() = default;

DynamicOrganizer::DynamicOrganizer(idp_t idp, std::string const& module) :
    _idp{idp},
    _module{utils::get_filename(module)},
    _members{},
    _next_idm{0}
{
    std::cout << "Criado um um objeto da classe \"DynamicOrganizer\", com idp: " << _idp << std::endl;
    if (!module.empty())
        global::pod->LoadModule(module);
}

DynamicOrganizer::~DynamicOrganizer() = default;

// DynamicOrganizer::DynamicOrganizer(DynamicOrganizer&&) noexcept = default;

// DynamicOrganizer& DynamicOrganizer::operator=(DynamicOrganizer&&) noexcept = default;



void DynamicOrganizer::Join(idp_t idp, std::string const& name)
{   
    // std::cout << "Join DYNAMIC ORGANIZER ---------------------------------------------" << std::endl;

    // verify if the resource has already been attached
    if (_members.find(idp) != _members.end())
        throw std::logic_error("Resource " + std::to_string(idp) + " already joined!");

    // verify if the name of the resource is unique in the context of the ancestor
    for (const auto& p: _members) {
        if (p.second.second == name)
            throw std::logic_error("Resource name <" + name + "> is not unique in the context of the ancestor!");
    }

    // C++ 17
    // for (const auto& [key, value] : _members) {
    //     auto&& [my_idm, my_name] = value;
    //     if (my_name == name) {
    //         throw std::logic_error("Resource name <" + name + "> is not unique in the context of the ancestor!");
    //     }
    // }

    // generate new idm
    auto idm = _next_idm++;

    // insert new resource
    if (name.empty())
        _members.emplace(idp, std::make_pair(idm, std::to_string(idp)));
    else
        _members.emplace(idp, std::make_pair(idm, name));
}

void DynamicOrganizer::Leave(idp_t idp)
{
    // erase resource idp
    _members.erase(idp);
}

std::string DynamicOrganizer::GetModuleName() const
{
    return _module;
}

std::size_t DynamicOrganizer::GetTotalMembers() const
{
    return _members.size();
}

std::vector<idp_t> DynamicOrganizer::GetMemberList() const
{
    std::vector<idp_t> list;

    for (auto const& member: _members)
        list.push_back(member.first);

    return list;
}

idp_t DynamicOrganizer::GetIdp(idm_t idm) const
{
    auto it = std::find_if(_members.begin(), _members.end(),
        [idm](auto&& member) -> bool {
            return member.second.first == idm;
        });

    if (it != _members.end()) {
        auto idp = it->first;
        return idp;
    } else {
        throw std::runtime_error("Resource with idm <" + std::to_string(idm) + "> does not exist!");
    }
}

idp_t DynamicOrganizer::GetIdp(std::string const& name) const
{
    auto it = std::find_if(_members.begin(), _members.end(),
        [name](auto&& member) -> bool {
            return member.second.second == name;
        });

    if (it != _members.end()) {
        auto idp = it->first;
        return idp;
    } else {
        throw std::runtime_error("Resource with name <" + name + "> does not exist!");
    }
}

idm_t DynamicOrganizer::GetIdm(idp_t idp) const
{
    auto idm = _members.at(idp).first;
    return idm;
}

idm_t DynamicOrganizer::GetIdm(std::string const& name) const
{
    auto it = std::find_if(_members.begin(), _members.end(),
        [name](auto&& member) -> bool {
            return member.second.second == name;
        });

    if (it != _members.end()) {
        auto idm = it->second.first;
        return idm;
    } else {
        throw std::runtime_error("Resource with name <" + name + "> does not exist!");
    }
}

idp_t DynamicOrganizer::GetDynamicIdp() const 
{
    return _idp;
}





}
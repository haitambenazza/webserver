/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:33:31 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/29 20:48:42 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_HPP
#define BLOCK_HPP
#include "Includes.hpp"

class Block
{
    private :
        int                         Lvl;
        size_t                      ArgStart;
        std::string                 BlockName;
        std::string                 Arg;
        std::vector<Block>          Blocks;
        std::vector<std::string>    Names;
        Block*                      Parent;
        bool                        Status;
    public:
        Block();
        Block( const Block& copy );
        Block& operator=( const Block& copy );
        ~Block();
        
        std::vector<Block>&         GetBlocks();
        void                        SetBlockName( std::string& s, Block& block, size_t pos );
        const std::string&          GetName( ) const;
        void                        FillBlock( std::string& s,Block& block, int& i, int& j );
        void                        InBrakects( std::string& s, size_t pos, Block &blk );
        void                        GetNames( std::string& s, Block& block );
        bool                        HasParent() const;
        std::string&                GetArg();
        int                         GetLvl();
        void                        ArgEpur(std::string &s, Block& blk );
        bool                        GetStatus() const;
};  

#endif
/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 Jean-David Gadina - www-xs-labs.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

/*!
 * @header      MachO-Segment.cpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#include "MachO-Segment.hpp"
#include "MachO-Header.hpp"
#include "BinaryStream.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

#define DCE_MACH_O_SEGMENT_NAME_SIZE    16

template<>
class XS::PIMPL::Object< DCE::MachO::Segment >::IMPL
{
    public:
        
        IMPL( void );
        IMPL( const IMPL & o );
        ~IMPL( void );
        
        uint32_t _command;
        uint32_t _commandSize;
        char     _name[ DCE_MACH_O_SEGMENT_NAME_SIZE ];
        uint64_t _vmAddress;
        uint64_t _vmSize;
        uint64_t _fileOffset;
        uint64_t _fileSize;
        uint32_t _maxProt;
        uint32_t _initProt;
        uint32_t _sectionsCount;
        uint32_t _flags;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#define XS_PIMPL_CLASS DCE::MachO::Segment
#include <XS/PIMPL/Object-IMPL.hpp>

namespace DCE
{
    namespace MachO
    {
        bool Segment::Read( const Header & header, BinaryStream & stream )
        {
            if( stream.IsGood() == false || stream.IsEOF() )
            {
                return false;
            }
            
            this->impl->_command     = stream.ReadUnsignedInteger();
            this->impl->_commandSize = stream.ReadUnsignedInteger();
            
            stream.Read( this->impl->_name, DCE_MACH_O_SEGMENT_NAME_SIZE );
            
            if( stream.IsEOF() )
            {
                return false;
            }
            
            if( header.Is64Bits() )
            {
                this->impl->_vmAddress  = stream.ReadUnsignedLong();
                this->impl->_vmSize     = stream.ReadUnsignedLong();
                this->impl->_fileOffset = stream.ReadUnsignedLong();
                this->impl->_fileSize   = stream.ReadUnsignedLong();
            }
            else if( header.Is32Bits() )
            {
                this->impl->_vmAddress  = stream.ReadUnsignedInteger();
                this->impl->_vmSize     = stream.ReadUnsignedInteger();
                this->impl->_fileOffset = stream.ReadUnsignedInteger();
                this->impl->_fileSize   = stream.ReadUnsignedInteger();
            }
            else
            {
                return false;
            }
            
            this->impl->_maxProt       = stream.ReadUnsignedInteger();
            this->impl->_initProt      = stream.ReadUnsignedInteger();
            this->impl->_sectionsCount = stream.ReadUnsignedInteger();
            
            if( stream.IsEOF() )
            {
                return false;
            }
            
            this->impl->_flags = stream.ReadUnsignedInteger();
            
            return true;
        }
        
        uint32_t Segment::GetCommand( void ) const
        {
            return this->impl->_command;
        }
        
        uint32_t Segment::GetCommandSize( void ) const
        {
            return this->impl->_commandSize;
        }
        
        std::string Segment::GetName( void ) const
        {
            return std::string( this->impl->_name, DCE_MACH_O_SEGMENT_NAME_SIZE );
        }
        
        uint64_t Segment::GetVMAddress( void ) const
        {
            return this->impl->_vmAddress;
        }
        
        uint64_t Segment::GetVMSize( void ) const
        {
            return this->impl->_vmSize;
        }
        
        uint64_t Segment::GetFileOffset( void ) const
        {
            return this->impl->_fileOffset;
        }
        
        uint64_t Segment::GetFileSize( void ) const
        {
            return this->impl->_fileSize;
        }
        
        uint32_t Segment::GetMaxProt( void ) const
        {
            return this->impl->_maxProt;
        }
        
        uint32_t Segment::GetInitProt( void ) const
        {
            return this->impl->_initProt;
        }
        
        uint32_t Segment::GetSectionsCount( void ) const
        {
            return this->impl->_sectionsCount;
        }
        
        uint32_t Segment::GetFlags( void ) const
        {
            return this->impl->_flags;
        }
    }
}

XS::PIMPL::Object< DCE::MachO::Segment >::IMPL::IMPL( void ):
    _command( 0 ),
    _commandSize( 0 ),
    _vmAddress( 0 ),
    _vmSize( 0 ),
    _fileOffset( 0 ),
    _fileSize( 0 ),
    _maxProt( 0 ),
    _initProt( 0 ),
    _sectionsCount( 0 ),
    _flags( 0 )
{
    memset( this->_name, 0, DCE_MACH_O_SEGMENT_NAME_SIZE );
}

XS::PIMPL::Object< DCE::MachO::Segment >::IMPL::IMPL( const IMPL & o ):
    _command( o._command ),
    _commandSize( o._commandSize ),
    _vmAddress( o._vmAddress ),
    _vmSize( o._vmSize ),
    _fileOffset( o._fileOffset ),
    _fileSize( o._fileSize ),
    _maxProt( o._maxProt ),
    _initProt( o._initProt ),
    _sectionsCount( o._sectionsCount ),
    _flags( o._flags )
{
    memcpy( this->_name, o._name, DCE_MACH_O_SEGMENT_NAME_SIZE );
}

XS::PIMPL::Object< DCE::MachO::Segment >::IMPL::~IMPL( void )
{}
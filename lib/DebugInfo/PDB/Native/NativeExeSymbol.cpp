//===- NativeExeSymbol.cpp - native impl for PDBSymbolExe -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/DebugInfo/PDB/Native/NativeExeSymbol.h"

#include "llvm/DebugInfo/PDB/Native/DbiStream.h"
#include "llvm/DebugInfo/PDB/Native/InfoStream.h"
#include "llvm/DebugInfo/PDB/Native/NativeEnumModules.h"
#include "llvm/DebugInfo/PDB/Native/PDBFile.h"

namespace llvm {
namespace pdb {

NativeExeSymbol::NativeExeSymbol(NativeSession &Session)
    : NativeRawSymbol(Session), File(Session.getPDBFile()) {}

std::unique_ptr<IPDBEnumSymbols>
NativeExeSymbol::findChildren(PDB_SymType Type) const {
  switch (Type) {
  case PDB_SymType::Compiland: {
    auto Dbi = File.getPDBDbiStream();
    if (Dbi) {
      const auto Modules = Dbi->modules();
      return std::unique_ptr<IPDBEnumSymbols>(
          new NativeEnumModules(Session, Modules));
    }
    consumeError(Dbi.takeError());
    break;
  }
  default:
    break;
  }
  return nullptr;
}

uint32_t NativeExeSymbol::getAge() const {
  auto IS = File.getPDBInfoStream();
  if (IS)
    return IS->getAge();
  consumeError(IS.takeError());
  return 0;
}

std::string NativeExeSymbol::getSymbolsFileName() const {
  return File.getFilePath();
}

PDB_UniqueId NativeExeSymbol::getGuid() const {
  auto IS = File.getPDBInfoStream();
  if (IS)
    return IS->getGuid();
  consumeError(IS.takeError());
  return PDB_UniqueId{{0}};
}

bool NativeExeSymbol::hasCTypes() const {
  auto Dbi = File.getPDBDbiStream();
  if (Dbi)
    return Dbi->hasCTypes();
  consumeError(Dbi.takeError());
  return false;
}

bool NativeExeSymbol::hasPrivateSymbols() const {
  auto Dbi = File.getPDBDbiStream();
  if (Dbi)
    return !Dbi->isStripped();
  consumeError(Dbi.takeError());
  return false;
}

} // namespace pdb
} // namespace llvm

/*
 Copyright (c) 2017, Simon Geilfus
 All rights reserved.
 
 This code is designed for use with the Cinder C++ library, http://libcinder.org
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:
    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once

#include <memory>
#include <functional>

#include "cinder/Exception.h"
#include "cinder/Filesystem.h"
#include "cinder/signals.h"

using ProcessPtr = std::unique_ptr<class Process>;

namespace runtime {

class CompilationResult;

class CompilerBase {
public:
	CompilerBase();
	~CompilerBase();
	
	//! Compiles and links the file at path. A callback can be specified to get the compilation results.
	void build( const ci::fs::path &path, const std::function<void(const CompilationResult&)> &onBuildFinish = nullptr );
	//! Compiles and links the file at path. A callback can be specified to get the compilation results.
	void build( const ci::fs::path &path, const std::string &compilerArgs, const std::string &linkerArgs, const std::function<void(const CompilationResult&)> &onBuildFinish = nullptr );

	std::string		getCLInitCommand() const;
	ci::fs::path	getCompilerPath() const;
	ci::fs::path	getProcessPath() const;
	
protected:
	virtual void initializeProcess();

	ProcessPtr		mProcess;
	std::string		mCLInitCommand;
	ci::fs::path	mCompilerPath;
	ci::fs::path	mProcessPath;
};

class CompilationResult {
public:
	//! Returns the path of the file that has been compiled
	ci::fs::path getFilePath() const;
	//! Returns the path of the compilation output
	ci::fs::path getOutputPath() const;
	//! Returns whether the compilation ended with errors
	bool hasErrors() const;
	//! Returns the list of errors 
	const std::vector<std::string>& getErrors() const;
	//! Returns the list of warnings
	const std::vector<std::string>& getWarnings() const;
	//! Returns a map of exported symbols and their mangled names
	const std::map<std::string,std::string>& getSymbols() const;
	
	CompilationResult( const ci::fs::path &filePath, const ci::fs::path &outputPath, const std::vector<std::string>& errors, const std::vector<std::string>& warnings, const std::map<std::string,std::string> &symbols );

protected:
	ci::fs::path mOutputPath;
	ci::fs::path mFilePath;
	std::vector<std::string> mErrors;
	std::vector<std::string> mWarnings;
	std::map<std::string,std::string> mSymbols;
};

class CompilerException : public ci::Exception {
public:
	CompilerException( const std::string &message ) : ci::Exception( message ) {}
};

} // namespace runtime

namespace rt = runtime;
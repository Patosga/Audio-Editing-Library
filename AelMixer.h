//
//  AelMixer.h
//  AudioEditingLibrary
//
//  Created by José Martins on 31/05/14.
//  Copyright (c) 2014 José Martins. All rights reserved.
//

#ifndef __AudioEditingLibrary__AelMixer__
#define __AudioEditingLibrary__AelMixer__

#include "AelEffects.h"
#include "AelDynamicEffect.h"
#include <list>

namespace Ael {

//////////////////////////////////////////////////////////////////////////////
// Classe AelChannel
// Representa um Canal da Mixer
// Variáveis-Membro:
//   ID(static int), channel_ID(const int), name(string), volume(AelVolume)
//   panner(AelPanner), stream(AelAudioStream), effectChain(list<AelEffect*>)
//   onoff(bool)
// Funções-Membro:
//   1 Construtores
//   Métodos que retornam/atribuem estado das variáveis membro
//   Métodos que adicionam/removem Efeitos ao canal
//   Método que retorna próxima frame do canal processada
//   Método que processa todas as frames do canal e retorna stream processada
//////////////////////////////////////////////////////////////////////////////
    
    class AelChannel {
        
        friend class AelMixer;
        static int ID;
        
        const int channel_ID;
        string name;
        AelVolume volume;
        AelPanner panner;
        AelAudioStream stream;
        list<AelEffect*> effectChain;
        bool onoff;
        
    public:
        
        AelChannel(const string &fileName) : stream(fileName), volume(1.0), panner(0), onoff(true), name(fileName),
        channel_ID(ID++) { }
        
        void setVolumeDb(double volDb) { volume.setVolumeDb(volDb); }
        void setPan(double pan){ panner.setPan(pan); }
        double getVolumeDb() { return volume.getVolumeDb(); }
        double getPan() { return panner.getPan(); }
        
        void turnOn() { onoff = true; }
        void turnOff() { onoff = false; }
        bool isOn() { return onoff; }
        
        int getID() { return channel_ID; }
        string getName() { return name; }
        void setName(string newname) { name = newname; }

        int addEffect(AelEffect &effect) { effectChain.push_back(&effect); return effect.getId();}
        bool removeEffect(int effectId);
        AelEffect* getEffect(int effectId);
        
        AelFrame getNextFrame();
        AelAudioStream* getFullyProcessed();
        
        bool isEOC(){ return stream.isEOS(); }
        
    };
    

///////////////////////////////////////////////////////////////////////////////
// Classe AelMixer
// Representa a Mixer da Biblioteca
// Variáveis-Membro:
//   m_nchannels(int), m_nMaxFrames(int), currPos(int),
//   masterVolDb(AelVolume), masterPan(AelPanner),
//   channel_list (list<AelChannel*>), master_effects(list<AelEffect*>)
// Funções-Membro:
//   1 Construtores
//   Métodos que adicionam/removem/retornam Canais e Efeitos da mixer
//   Método que retorna próxima frame do canal processada
//   Método que processa todas as frames do canal e retorna stream processada
//   Métodos de posicionamento da mixer em msegundos/número de frames
//   Métodos que retornam/atribuem estado das variáveis membro
//////////////////////////////////////////////////////////////////////////////
    class AelMixer{
        
    public:
        
        AelMixer() : m_nChannels(0), masterVolDb(1.0), masterPan(0.0), m_nMaxFrames(0), currPos(0) { }
        
        //AelChannel* addChannel(const string &filename);
        AelChannel* addChannel(AelChannel *newchannel);

        AelChannel* getChannel(const int &channelID);
        bool removeChannel(const int &channelID);
        
        
        AelFrame getNextFrame();
        AelAudioStream* getFullMix();
        
        AelEffect* getEffect(int effectId);
        int addEffect(AelEffect &effect) { master_effects.push_back(&effect); return effect.getId(); }
        bool removeEffect(int effectId);

        void setPosMsec(int mseg);
        int getPosMsec() { return static_cast<float>(currPos) * 1000.0 / 44100.0; }
        void setPosFrames(int nframe);
        int getPosFrames() { return currPos; }
        
        int getLengthMsec() { return static_cast<float>(m_nMaxFrames) * 1000.0 / 44100.0; }
        int getLengthFrames() { return m_nMaxFrames; }
        
        int getnChannels() { return m_nChannels; }

        void setVolumeDb(double volDb) { masterVolDb.setVolumeDb(volDb); }
        void setPan(double pan){ masterPan.setPan(pan); }
        double getVolumeDb() { return masterVolDb.getVolumeDb(); }
        double getPan() { return masterPan.getPan(); }
        bool isEOM() { return currPos == m_nMaxFrames; }
        
    
        
    private:
        
        int m_nChannels;
        list<AelChannel*> channel_list;
        list<AelEffect*> master_effects;
        AelVolume masterVolDb;
        AelPanner masterPan;
        int m_nMaxFrames;
        int currPos;
        list<AelChannel*>::iterator findChannel(const int &channelID);
    };
    
}


#endif /* defined(__AudioEditingLibrary__AelMixer__) */

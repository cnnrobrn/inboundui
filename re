import React, { useState } from 'react';
import { Camera, Upload, Loader } from 'lucide-react';
import { Alert, AlertDescription } from '@/components/ui/alert';
import { Button } from '@/components/ui/button';
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card';

const HarveyBall = ({ score }) => {
  const getColor = (score) => {
    if (score < 50) return 'text-red-500';
    if (score < 75) return 'text-yellow-500';
    return 'text-green-500';
  };

  const createArc = (percentage) => {
    const radius = 45;
    const center = 50;
    const angle = (percentage / 100) * 360;
    const angleInRadians = (angle - 90) * (Math.PI / 180);
    const x = center + radius * Math.cos(angleInRadians);
    const y = center + radius * Math.sin(angleInRadians);
    const largeArcFlag = angle > 180 ? 1 : 0;
    
    if (percentage === 0) return '';
    if (percentage === 100) return `M50,50 m-45,0 a45,45 0 1,0 90,0 a45,45 0 1,0 -90,0`;
    return `M50,50 L50,5 A45,45 0 ${largeArcFlag},1 ${x},${y} Z`;
  };

  const colorClass = getColor(score);
  
  return (
    <svg viewBox="0 0 100 100" className={`w-6 h-6 ${colorClass}`}>
      <path
        d="M50,50 m-45,0 a45,45 0 1,0 90,0 a45,45 0 1,0 -90,0"
        fill="none"
        stroke="currentColor"
        strokeWidth="10"
      />
      <path
        d={createArc(score)}
        fill="currentColor"
      />
    </svg>
  );
};

const ScoreItem = ({ label, score, locked }) => (
  <Card className={`bg-white/50 backdrop-blur-sm border border-gray-200 shadow-sm hover:shadow-md transition-all duration-200 ${locked ? 'opacity-50' : ''}`}>
    <CardContent className="flex items-center justify-between p-3">
      <span className="text-sm font-medium text-gray-700 min-w-0 flex-shrink truncate mr-2">{label}</span>
      <div className={`flex items-center gap-2 flex-shrink-0 ${locked ? 'blur-sm' : ''}`}>
        <HarveyBall score={locked ? 0 : score} />
        <span className="text-sm font-medium text-gray-600 w-9">{locked ? '--' : `${score}%`}</span>
      </div>
    </CardContent>
  </Card>
);

const MobileCameraApp = () => {
  const [step, setStep] = useState('capture');
  const [imageData, setImageData] = useState(null);
  const [results, setResults] = useState(null);

  const mockResults = {
    scores: [
      { label: "Clarity", value: 85 },
      { label: "Contrast", value: 45 },
      { label: "Balance", value: 92 },
      { label: "Detail", value: 67 },
      { label: "Color", value: 78 },
      { label: "Focus", value: 88 }
    ],
    summary: "Perfect Match Found",
    description: "The analyzed image shows exceptional characteristics matching our criteria. The pattern recognition algorithm identified key features that align with our database entries.",
    learnMoreUrl: "https://example.com/learn-more"
  };

  const handleCapture = (e) => {
    const file = e.target.files[0];
    if (file) {
      const reader = new FileReader();
      reader.onloadend = () => {
        setImageData(reader.result);
        setStep('loading');
        setTimeout(() => {
          setResults(mockResults);
          setStep('results');
        }, 2000);
      };
      reader.readAsDataURL(file);
    }
  };

  const renderStep = () => {
    switch (step) {
      case 'capture':
        return (
          <div className="flex flex-col items-center justify-center space-y-4">
            <div className="relative w-64 h-64 bg-gradient-to-br from-gray-50 to-gray-100 rounded-2xl flex items-center justify-center border-2 border-dashed border-gray-300 hover:border-gray-400 transition-colors duration-200">
              <Camera className="w-24 h-24 text-gray-400" />
              <input
                type="file"
                accept="image/*"
                capture="environment"
                onChange={handleCapture}
                className="absolute inset-0 w-full h-full opacity-0 cursor-pointer"
              />
            </div>
            <p className="text-gray-600 font-medium">Tap to take a photo</p>
          </div>
        );

      case 'loading':
        return (
          <div className="flex flex-col items-center justify-center space-y-4">
            <div className="w-64 h-64 rounded-2xl overflow-hidden relative shadow-lg">
              <img src={imageData} alt="Captured" className="w-full h-full object-cover" />
              <div className="absolute inset-0 bg-black/50 backdrop-blur-sm flex items-center justify-center">
                <Loader className="w-12 h-12 text-white animate-spin" />
              </div>
            </div>
            <p className="text-gray-600 font-medium animate-pulse">Analyzing image...</p>
          </div>
        );

      case 'results':
        return (
          <div className="flex flex-col items-center space-y-8">
            <div className="w-64 h-64 rounded-2xl overflow-hidden shadow-lg">
              <img src={imageData} alt="Analyzed" className="w-full h-full object-cover" />
            </div>
            
            <div className="w-full max-w-sm space-y-8">
              <div className="space-y-3">
                <p className="text-sm font-medium text-gray-600 text-center">It's giving:</p>
                <Alert className="border-2 bg-white/50 backdrop-blur-sm shadow-sm">
                  <AlertDescription className="font-semibold text-center">
                    {results.summary}
                  </AlertDescription>
                </Alert>
                <p className="text-gray-600 text-sm text-center leading-relaxed">
                  {results.description}
                </p>
              </div>

              <div className="grid grid-cols-2 gap-3">
                {results.scores.map((score, idx) => (
                  <ScoreItem 
                    key={idx} 
                    label={score.label} 
                    score={score.value}
                    locked={idx >= 2}  // Only first two scores are unlocked
                  />
                ))}
              </div>

              <Button 
                className="w-full bg-black hover:bg-gray-900 text-white shadow-md hover:shadow-lg transition-all duration-200"
                onClick={() => window.location.href = results.learnMoreUrl}
              >
                Get Your Full Recommendations
              </Button>
            </div>
          </div>
        );
    }
  };

  return (
    <Card className="w-full max-w-md mx-auto bg-gradient-to-b from-white to-gray-50 border-0 shadow-xl">
      <CardHeader className="border-b border-gray-100">
        <CardTitle className="text-center text-xl font-semibold bg-gradient-to-r from-gray-800 to-gray-600 bg-clip-text text-transparent">
          Image Analysis
        </CardTitle>
      </CardHeader>
      <CardContent className="p-6">
        {renderStep()}
      </CardContent>
    </Card>
  );
};

export default MobileCameraApp;
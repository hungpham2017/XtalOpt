/**********************************************************************
  SIESTAOptimizer - Tools to interface with SIESTA

  Copyright (C) 2009-2011 by David C. Lonie

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 ***********************************************************************/

#ifndef SIESTAOPTIMIZER_H
#define SIESTAOPTIMIZER_H

#include <xtalopt/optimizers/xtaloptoptimizer.h>

#include <QObject>

namespace GlobalSearch {
  class Structure;
  class OptBase;
  class Optimizer;
}

namespace XtalOpt {
  class SIESTAOptimizer : public XtalOptOptimizer
  {
    Q_OBJECT

   public:
    SIESTAOptimizer(GlobalSearch::OptBase *parent,
                  const QString &filename = "");

    QHash<QString, QString>
      getInterpretedTemplates(GlobalSearch::Structure *structure);


    void buildPSFs();
    bool PSFInfoIsUpToDate(QList<uint> comp);
  };

} // end namespace XtalOpt

#endif

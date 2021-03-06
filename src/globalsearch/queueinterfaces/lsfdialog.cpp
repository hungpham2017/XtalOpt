/**********************************************************************
  LsfConfigDialog -- Setup for remote LSF queues

  Copyright (C) 2011 by David Lonie

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 ***********************************************************************/

#ifdef ENABLE_SSH

// Doxygen skip:
/// @cond

#include <globalsearch/queueinterfaces/lsfdialog.h>

#include <globalsearch/queueinterfaces/lsf.h>

#include <globalsearch/ui/abstractdialog.h>
#include <globalsearch/optbase.h>

#include "ui_lsfdialog.h"

namespace GlobalSearch {

  LsfConfigDialog::LsfConfigDialog(AbstractDialog *parent,
                                   OptBase *o,
                                   LsfQueueInterface *p)
    : QDialog(parent),
      m_opt(o),
      m_lsf(p),
      ui(new Ui::LsfConfigDialog)
  {
    ui->setupUi(this);
  }

  LsfConfigDialog::~LsfConfigDialog()
  {
    delete ui;
  }

  void LsfConfigDialog::updateGUI()
  {
    ui->edit_description->blockSignals(true);
    ui->edit_host->blockSignals(true);
    ui->edit_bkill->blockSignals(true);
    ui->edit_bjobs->blockSignals(true);
    ui->edit_bsub->blockSignals(true);
    ui->edit_rempath->blockSignals(true);
    ui->edit_locpath->blockSignals(true);
    ui->edit_username->blockSignals(true);
    ui->spin_port->blockSignals(true);
    ui->cb_cleanRemoteOnStop->blockSignals(true);
    ui->cb_logErrorDirs->blockSignals(true);

    ui->edit_description->setText(m_opt->description);
    ui->edit_host->setText(m_opt->host);
    ui->edit_bkill->setText(m_lsf->m_cancelCommand);
    ui->edit_bjobs->setText(m_lsf->m_statusCommand);
    ui->edit_bsub->setText(m_lsf->m_submitCommand);
    ui->edit_rempath->setText(m_opt->rempath);
    ui->edit_locpath->setText(m_opt->filePath);
    ui->edit_username->setText(m_opt->username);
    ui->spin_port->setValue(m_opt->port);
    ui->cb_cleanRemoteOnStop->setChecked(m_lsf->m_cleanRemoteOnStop);
    ui->cb_logErrorDirs->setChecked(m_opt->m_logErrorDirs);

    ui->edit_description->blockSignals(false);
    ui->edit_host->blockSignals(false);
    ui->edit_bkill->blockSignals(false);
    ui->edit_bjobs->blockSignals(false);
    ui->edit_bsub->blockSignals(false);
    ui->edit_rempath->blockSignals(false);
    ui->edit_locpath->blockSignals(false);
    ui->edit_username->blockSignals(false);
    ui->spin_port->blockSignals(false);
    ui->cb_cleanRemoteOnStop->blockSignals(false);
    ui->cb_logErrorDirs->blockSignals(false);
  }

  void LsfConfigDialog::accept()
  {
    m_opt->description = ui->edit_description->text().trimmed();
    m_opt->host = ui->edit_host->text().trimmed();
    m_lsf->m_cancelCommand = ui->edit_bkill->text().trimmed();
    m_lsf->m_statusCommand = ui->edit_bjobs->text().trimmed();
    m_lsf->m_submitCommand = ui->edit_bsub->text().trimmed();
    m_opt->rempath = ui->edit_rempath->text().trimmed();
    m_opt->filePath = ui->edit_locpath->text().trimmed();
    m_opt->username = ui->edit_username->text().trimmed();
    m_opt->port = ui->spin_port->value();
    m_lsf->m_cleanRemoteOnStop = ui->cb_cleanRemoteOnStop->isChecked();
    m_opt->m_logErrorDirs = ui->cb_logErrorDirs->isChecked();
    QDialog::accepted();
    close();
  }

  void LsfConfigDialog::reject()
  {
    updateGUI();
    QDialog::reject();
    close();
  }

}

/// @endcond
#endif // ENABLE_SSH

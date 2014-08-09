/**
 * Copyright (c) 2011-2014 sx developers (see AUTHORS)
 *
 * This file is part of sx.
 *
 * sx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <sx/command/tx-encode.hpp>

#include <iostream>
#include <vector>
#include <bitcoin/bitcoin.hpp>
#include <sx/serializer/input.hpp>
#include <sx/serializer/output.hpp>
#include <sx/serializer/transaction.hpp>
#include <sx/utility/utility.hpp>

using namespace bc;
using namespace sx;
using namespace sx::extension;
using namespace sx::serializer;

console_result tx_encode::invoke(std::ostream& output, std::ostream& cerr)
{
    // Bound parameters.
    const auto& inputs = get_inputs_option();
    const auto& outputs = get_outputs_option();
    const auto locktime = get_locktime_option();
    const auto version = get_version_option();
    const auto& file = get_transaction_argument();

    transaction_type tx;
    tx.version = version;
    tx.locktime = locktime;

    for (const transaction_input_type& input: inputs)
        tx.inputs.push_back(input);

    for (const std::vector<transaction_output_type>& output_sets: outputs)
        for (const auto& output: output_sets)
            tx.outputs.push_back(output);

    if (is_locktime_conflict(tx))
        cerr << SX_TX_ENCODE_LOCKTIME_CONFLICT << std::endl;

    write_output(output, file, transaction(tx));

    return console_result::okay;
}